/*
				GNU GENERAL PUBLIC LICENSE
				   Version 3, June 1991
COPYRIGHT (C) Bestkakkoii 2024 All Rights Reserved.
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/

#include <pch.h>
#include <cxxastardevice_p.h>

#include <cxxmath.h>

#pragma region Private
CXXAStarDevicePrivate::CXXAStarDevicePrivate(CXXAStarDevice* ptr)
	: q_ptr(ptr)
	, rng_(std::random_device{}())
	, dist_(0.0, 1.0)
	, pathVariationFactor_(0.2)
	, stepsBeforeVariation_(15)
	, currentStep_(0)
	, maxAlternativeGoals_(5)
	, maxDeviationFactor_(0.3)
{
}

CXXAStarDevicePrivate::~CXXAStarDevicePrivate()
{
}

// 清理参数
void CXXAStarDevicePrivate::clear()
{
	// 释放 mapping_ 中的节点内存
	for (CXXNode*& node : record_)
	{
		if (node != CXX_NULLPTR)
		{
			std::allocator_traits<std::pmr::polymorphic_allocator<CXXNode>>::destroy(*allocator_, node);
			allocator_->deallocate(node, 1);
		}
	}

	open_list_.clear();
	record_.clear();
	RtlSecureZeroMemory(mapping_.data(), mapping_.size() * sizeof(CXXNode*));
}

// 参数是否有效
bool CXXAStarDevicePrivate::isValidParams(const CXXPoint& start, const CXXPoint& end) const
{
	return ((can_pass_ != CXX_NULLPTR)
		&& ((start.x() >= 0) && (start.x() < width_))
		&& ((start.y() >= 0) && (start.y() < height_))
		&& ((end.x() >= 0) && (end.x() < width_))
		&& ((end.y() >= 0) && (end.y() < height_))
		&& width_ > 0 && height_ > 0);
}

// 获取节点索引
bool CXXAStarDevicePrivate::getNodeIndex(CXXNode*& node, __int64* index)
{
	*index = 0;
	__int64 size = static_cast<__int64>(open_list_.size());
	while (*index < size)
	{
		if (open_list_[*index]->pos == node->pos)
		{
			return true;
		}
		++(*index);
	}

	return false;
}

// 二叉堆上滤
void CXXAStarDevicePrivate::percolateUp(__int64& hole)
{
	__int64 parent = 0;
	while (hole > 0)
	{
		parent = (hole - 1) / 2;
		if (open_list_[hole]->f() < open_list_[parent]->f())
		{
			std::swap(open_list_[hole], open_list_[parent]);
			hole = parent;
		}
		else
		{
			return;
		}
	}
}

#if defined(Chebyshev_distance)
static __forceinline __int64 Chebyshev_Distance(const CXXPoint& current, const CXXPoint& end)
{
	return std::max(std::abs(current.x() - end.x()), std::abs(current.y() - end.y()));
}
#elif defined(Euclidean_distance)
static __forceinline __int64 Euclidean_Distance(const CXXPoint& current, const CXXPoint& end)
{
	return std::sqrt(std::pow(current.x() - end.x(), 2) + std::pow(current.y() - end.y(), 2));
}
#elif defined(Octile_distance)
static __forceinline __int64 Octile_Distance(const CXXPoint& current, const CXXPoint& end)
{
	quint64 dx = std::abs(current.x() - end.x());
	quint64 dy = std::abs(current.y() - end.y());

	if (dx > dy)
	{
		return kStepValue * dx + (kObliqueValue - kStepValue) * dy;
	}

	return kStepValue * dy + (kObliqueValue - kStepValue) * dx;
}
#else 
static __forceinline __int64 Manhattan_Distance(const CXXPoint& current, const CXXPoint& end)
{
	return (current - end).manhattanLength();
}
#endif
// 计算G值

// 节点是否存在于开启列表
__forceinline bool CXXAStarDevicePrivate::isInOpenList(const CXXPoint& pos, CXXNode*& out_node)
{
	out_node = mapping_[static_cast<size_t>(pos.y()) * width_ + pos.x()];

	return out_node ? out_node->state == NodeState::IN_OPENLIST : false;
}

// 节点是否存在于关闭列表
__forceinline bool CXXAStarDevicePrivate::isInCloseList(const CXXPoint& pos)
{
	CXXNode* node_ptr = mapping_[static_cast<size_t>(pos.y()) * width_ + pos.x()];

	return node_ptr ? node_ptr->state == NodeState::IN_CLOSEDLIST : false;
}

double CXXAStarDevicePrivate::getActualCost(const CXXPoint& pos)
{
	if (pos.x() < 0 || pos.x() >= width_ || pos.y() < 0 || pos.y() >= height_)
	{
		return -1.0;
	}

	return can_pass_(pos);
}

// 是否可到达
bool CXXAStarDevicePrivate::isPassible(const CXXPoint& pos)
{
	if (pos.x() < 0 || pos.x() >= width_ || pos.y() < 0 || pos.y() >= height_)
	{
		return false;
	}

	double cost = can_pass_(pos);

	return cost >= 0.0;
}

// 当前点是否可到达目标点
bool CXXAStarDevicePrivate::isPassible(const CXXPoint& current, const CXXPoint& destination, const bool& allow_corner, double& cost)
{
	if (destination.x() < 0 || destination.x() >= width_ || destination.y() < 0 || destination.y() >= height_)
	{
		return false;
	}

	if (isInCloseList(destination))
	{
		return false;
	}

	double tileCost = getActualCost(destination);
	if (tileCost <= -1.0)
	{
		return false;  // 不可通过的地块
	}

	__int64 dx = static_cast<__int64>(destination.x() - current.x());
	__int64 dy = static_cast<__int64>(destination.y() - current.y());

	if (std::abs(dx) + std::abs(dy) == 1)
	{
		cost = ORTHOGONAL_COST * (tileCost < 0 ? AVOIDANCE_PENALTY : tileCost);
		return true;
	}

	if (!allow_corner || std::abs(dx) != 1 || std::abs(dy) != 1)
	{
		return false;
	}

	// 检查对角移动的两个相邻地块
	double cost1 = getActualCost(CXXPoint(current.x() + dx, current.y()));
	double cost2 = getActualCost(CXXPoint(current.x(), current.y() + dy));

	if (cost1 <= -1.0 || cost2 <= -1.0)
	{
		return false;
	}

	cost = DIAGONAL_COST * (tileCost < 0 ? AVOIDANCE_PENALTY : tileCost);

	return true;
}

__forceinline __int64 CXXAStarDevicePrivate::calculateGValue(CXXNode*& parent, const CXXPoint& current)
{
	double cost = 0.0;
	if (isPassible(parent->pos, current, corner_, cost))
	{
		return parent->g + static_cast<__int64>(cost);
	}

	return std::numeric_limits<__int64>::max();
}

// 计算F值
__forceinline __int64 CXXAStarDevicePrivate::calculateHValue(const CXXPoint& current, const CXXPoint& end)
{
	__int64 dx = std::abs(static_cast<__int64>(current.x() - end.x()));
	__int64 dy = std::abs(static_cast<__int64>(current.y() - end.y()));

	__int64 straight = std::abs(dx - dy);
	__int64 diagonal = std::min(dx, dy);

	__int64 base_h_value = straight * ORTHOGONAL_COST + diagonal * DIAGONAL_COST;

	// 增加对需要避开区域的惩罚
	__int64 avoidance_penalty = 0;
	for (__int64 y = std::max(0LL, current.y() - OBSTACLE_CHECK_RADIUS); y <= std::min(height_ - 1, current.y() + OBSTACLE_CHECK_RADIUS); ++y)
	{
		for (__int64 x = std::max(0LL, current.x() - OBSTACLE_CHECK_RADIUS); x <= std::min(width_ - 1, current.x() + OBSTACLE_CHECK_RADIUS); ++x)
		{
			double cost = getActualCost(CXXPoint(x, y));
			if (cost < 0 && cost > -1.0f)  // 检查是否是需要避开的区域
			{
				__int64 distance = std::max(std::abs(x - current.x()), std::abs(y - current.y()));
				avoidance_penalty += static_cast<__int64>(AVOIDANCE_PENALTY * OBSTACLE_PENALTY / (distance + 1));
			}
		}
	}

	base_h_value += avoidance_penalty;

	if (++currentStep_ % stepsBeforeVariation_ == 0)
	{
		double variation = dist_(rng_) * pathVariationFactor_ * 2.0 - pathVariationFactor_;
		base_h_value = static_cast<__int64>(base_h_value * (1.0 + variation));
	}

	return base_h_value;
}

// 处理找到节点的情况
void CXXAStarDevicePrivate::handleFoundNode(CXXNode*& current, CXXNode*& destination)
{
	__int64 g_value = calculateGValue(current, destination->pos);
	if (g_value < destination->g)
	{
		destination->g = g_value;
		destination->parent = current;

		__int64 index = 0;
		if (getNodeIndex(destination, &index))
		{
			percolateUp(index);
		}
		else
		{
			CXX_ASSERT(false);
		}
	}
}

// 处理未找到节点的情况
void CXXAStarDevicePrivate::handleFoundNodeFail(CXXNode*& current, CXXNode*& destination, const CXXPoint& end)
{
	double cost = 0.0;
	if (!isPassible(current->pos, destination->pos, corner_, cost))
	{
		return;
	}

	destination->parent = current;
	destination->h = calculateHValue(destination->pos, end);
	destination->g = calculateGValue(current, destination->pos);

	// Add obstacle avoidance cost
	__int64 obstacle_cost = 0;
	for (__int64 y = std::max(0LL, destination->pos.y() - OBSTACLE_CHECK_RADIUS); y <= std::min(height_ - 1, destination->pos.y() + OBSTACLE_CHECK_RADIUS); ++y)
	{
		for (__int64 x = std::max(0LL, destination->pos.x() - OBSTACLE_CHECK_RADIUS); x <= std::min(width_ - 1, destination->pos.x() + OBSTACLE_CHECK_RADIUS); ++x)
		{
			if (getActualCost(CXXPoint(x, y)) < 0)
			{
				__int64 distance = std::max(std::abs(x - destination->pos.x()), std::abs(y - destination->pos.y()));
				obstacle_cost += OBSTACLE_PENALTY / (distance + 1);
			}
		}
	}

	destination->g += obstacle_cost;

	destination->state = NodeState::IN_OPENLIST;
	mapping_[static_cast<size_t>(destination->pos.y()) * width_ + destination->pos.x()] = destination;

	open_list_.emplace_back(destination);
	std::push_heap(open_list_.begin(), open_list_.end(), [](const CXXNode* a, const CXXNode* b)->bool
		{
			return a->f() > b->f();
		}
	);
}

// 查找附近可通过的节点
void CXXAStarDevicePrivate::findPassibleNodes(const CXXPoint& current, const bool& corner, std::vector<CXXPoint>* out_lists)
{
	CXXPoint destination;
	__int64 row_index = std::max(0LL, current.y() - 1);
	const __int64 max_row = std::min(height_ - 1LL, current.y() + 1LL);
	const __int64 max_col = std::min(width_ - 1LL, current.x() + 1LL);

	std::vector<std::pair<CXXPoint, __int64>> potential_nodes;

	for (; row_index <= max_row; ++row_index)
	{
		for (__int64 col_index = std::max(0LL, current.x() - 1); col_index <= max_col; ++col_index)
		{
			destination.setX(col_index);
			destination.setY(row_index);
			double cost = 0.0;

			if (isPassible(current, destination, corner, cost))
			{
				__int64 obstacle_distance = std::numeric_limits<__int64>::max();
				for (__int64 y = std::max(0LL, destination.y() - OBSTACLE_CHECK_RADIUS); y <= std::min(height_ - 1, destination.y() + OBSTACLE_CHECK_RADIUS); ++y)
				{
					for (__int64 x = std::max(0LL, destination.x() - OBSTACLE_CHECK_RADIUS); x <= std::min(width_ - 1, destination.x() + OBSTACLE_CHECK_RADIUS); ++x)
					{
						if (getActualCost(CXXPoint(x, y)) < 0)
						{
							__int64 distance = std::max(std::abs(x - destination.x()), std::abs(y - destination.y()));
							obstacle_distance = std::min(obstacle_distance, distance);
						}
					}
				}

				potential_nodes.emplace_back(destination, obstacle_distance);
			}
		}
	}

	// Sort potential nodes by their distance from obstacles (descending order)
	std::sort(potential_nodes.begin(), potential_nodes.end(),
		[](const auto& a, const auto& b)->bool
		{
			return a.second > b.second;
		}
	);

	// Add sorted nodes to the output list
	for (const auto& node : potential_nodes)
	{
		out_lists->push_back(node.first);
	}

	// Shuffle the nodes that are equally far from obstacles
	auto it = std::stable_partition(out_lists->begin(), out_lists->end(),
		[&](const CXXPoint& a) ->bool
		{
			return std::find_if(potential_nodes.cbegin(), potential_nodes.cend(),
				[&](const auto& p)->bool
				{
					return p.first == a && p.second == potential_nodes.front().second;
				}
			) != potential_nodes.cend();
		}
	);

	std::shuffle(out_lists->begin(), it, rng_);
}

std::vector<CXXPoint> CXXAStarDevicePrivate::generateAlternativeGoals(const CXXPoint& start, const CXXPoint& end)
{
	std::vector<CXXPoint> goals;
	CXXPoint direction = end - start;
	double distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

	for (__int64 i = 0; i < maxAlternativeGoals_; ++i)
	{
		double deviation = dist_(rng_) * maxDeviationFactor_ * distance;
		double angle = dist_(rng_) * 2 * cxx::_PI;

		__int64 dx = static_cast<__int64>(deviation * std::cos(angle));
		__int64 dy = static_cast<__int64>(deviation * std::sin(angle));

		CXXPoint alt_goal = start + direction / 2 + CXXPoint(dx, dy);

		// 确保替代目标在网格范围内
		alt_goal.setX(std::clamp(static_cast<__int64>(alt_goal.x()), 0LL, static_cast<__int64>(width_) - 1LL));
		alt_goal.setY(std::clamp(static_cast<__int64>(alt_goal.y()), 0LL, static_cast<__int64>(height_) - 1LL));

		// 只有当目标可通过时才添加
		if (isPassible(alt_goal))
		{
			goals.push_back(alt_goal);
		}
	}

	// 如果没有找到任何替代目标，至少添加终点
	if (goals.empty())
	{
		goals.push_back(end);
	}

	return goals;
}

void CXXAStarDevicePrivate::setPathVariationFactor(double factor)
{
	pathVariationFactor_ = std::clamp(factor, 0.0, 1.0);
}

void CXXAStarDevicePrivate::setStepsBeforeVariation(__int64 steps)
{
	stepsBeforeVariation_ = std::max(1LL, steps);
}

void CXXAStarDevicePrivate::setAlternativeGoals(const std::vector<CXXPoint>& goals)
{
	alternativeGoals_ = goals;
}

void CXXAStarDevicePrivate::setDynamicGoalParams(__int64 max_goals, double max_deviation)
{
	maxAlternativeGoals_ = std::max(1LL, max_goals);
	maxDeviationFactor_ = std::clamp(max_deviation, 0.0, 1.0);
}

#pragma endregion

CXXAStarDevice::CXXAStarDevice()
	: d_ptr(new CXXAStarDevicePrivate(this))
{
}

CXXAStarDevice::~CXXAStarDevice()
{
	d_ptr->clear();
}

void CXXAStarDevice::initialize(__int64 width, __int64 height)
{
	std::lock_guard<std::mutex> lock(d_ptr->mutex_);
	if (d_ptr->height_ == height && d_ptr->width_ == width)
	{
		return;
	}

	d_ptr->height_ = height;
	d_ptr->width_ = width;

	d_ptr->resource_.reset(new std::pmr::monotonic_buffer_resource(d_ptr->width_ * d_ptr->height_ * sizeof(CXXNode) * 2));
	CXX_ASSERT(d_ptr->resource_ != CXX_NULLPTR);

	d_ptr->allocator_.reset(new std::pmr::polymorphic_allocator<CXXNode>(d_ptr->resource_.get()));
	CXX_ASSERT(d_ptr->allocator_ != CXX_NULLPTR);

	d_ptr->open_list_.clear();

	d_ptr->mapping_.clear();

	d_ptr->mapping_.resize(static_cast<size_t>(d_ptr->width_) * d_ptr->height_);
}

void CXXAStarDevice::setCanpass(const CXXAStarCallback& callback)
{
	d_ptr->can_pass_ = callback;
}

void CXXAStarDevice::setCorner(bool corner)
{
	d_ptr->corner_ = corner;
}

// 执行寻路操作
bool CXXAStarDevice::find(const CXXPoint& start, const CXXPoint& end, CXXVector<CXXPoint>* pPaths)
{
	std::lock_guard<std::mutex> lock(d_ptr->mutex_);
	d_ptr->currentStep_ = 0;  // 重置步数计数器

	if (pPaths != CXX_NULLPTR)
	{
		pPaths->clear();
	}

	if (!d_ptr->isValidParams(start, end))
	{
		d_ptr->clear();
		return false;
	}

	d_ptr->start_ = start;
	d_ptr->end_ = end;

	std::vector<CXXPoint> nearby_nodes;
	nearby_nodes.reserve(d_ptr->corner_ ? 8 : 4);
	constexpr size_t alloc_size(1u);

	// 将起点放入开启列表
	CXXNode* start_node = d_ptr->allocator_->allocate(alloc_size);  // 分配内存
	d_ptr->record_.emplace_back(start_node);
	std::allocator_traits<std::pmr::polymorphic_allocator<CXXNode>>::construct(*d_ptr->allocator_, start_node, d_ptr->start_);// 构造对象
	d_ptr->open_list_.emplace_back(start_node);
	start_node->state = NodeState::IN_OPENLIST;
	d_ptr->mapping_[static_cast<size_t>(start_node->pos.y()) * d_ptr->width_ + start_node->pos.x()] = start_node;

	CXXPoint current_goal = end;

	// 寻路操作
	while (!d_ptr->open_list_.empty())
	{
		// 找出f值最小节点
		CXXNode* current = CXX_NULLPTR;
		if (pPaths != CXX_NULLPTR && d_ptr->currentStep_ % d_ptr->stepsBeforeVariation_ == 0 && d_ptr->dist_(d_ptr->rng_) < d_ptr->pathVariationFactor_)
		{
			// 在特定步数时有机会选择次优路径
			size_t random_index = std::min<size_t>(static_cast<size_t>(d_ptr->dist_(d_ptr->rng_) * 3), d_ptr->open_list_.size() - 1);

			std::nth_element(d_ptr->open_list_.begin(), d_ptr->open_list_.begin() + random_index, d_ptr->open_list_.end(),
				[](const CXXNode* a, const CXXNode* b)
				{
					return a->f() < b->f();
				}
			);

			current = d_ptr->open_list_[random_index];
			d_ptr->open_list_.erase(d_ptr->open_list_.begin() + random_index);
		}
		else
		{
			// 大部分时间选择最优路径
			current = d_ptr->open_list_.front();

			std::pop_heap(d_ptr->open_list_.begin(), d_ptr->open_list_.end(),
				[](const CXXNode* a, const CXXNode* b)
				{
					return a->f() > b->f();
				}
			);

			d_ptr->open_list_.pop_back();
		}

		d_ptr->mapping_[static_cast<size_t>(current->pos.y()) * d_ptr->width_ + current->pos.x()]->state = NodeState::IN_CLOSEDLIST;

		// 是否找到当前目标
		if (current->pos == current_goal)
		{
			// 找到最终目标
			if (current_goal == end)
			{
				// 仅用于测试不实际返回路径
				if (CXX_NULLPTR == pPaths)
				{
					d_ptr->clear();
					return true;
				}

				// 生成路径 (此处是终点到起点)
				while (current->parent)
				{
					pPaths->append(current->pos);
					current = current->parent;
				}

				// 反转路径 (转换成起点到终点)
				std::reverse(pPaths->begin(), pPaths->end());
				break;
			}
			else
			{
				// 找到中间目标，现在瞄准最终目标
				current_goal = end;
			}
		}

		// 查找周围可通过节点
		nearby_nodes.clear();
		d_ptr->findPassibleNodes(current->pos, d_ptr->corner_, &nearby_nodes);

		// 计算周围节点的估值
		for (const auto& nearby_pos : nearby_nodes)
		{
			CXXNode* next_node = CXX_NULLPTR;
			if (d_ptr->isInOpenList(nearby_pos, next_node))
			{
				d_ptr->handleFoundNode(current, next_node);
			}
			else
			{
				next_node = d_ptr->allocator_->allocate(alloc_size);  // 分配内存
				d_ptr->record_.push_back(next_node);
				std::allocator_traits<std::pmr::polymorphic_allocator<CXXNode>>::construct(*d_ptr->allocator_, next_node, nearby_pos);// 构造对象
				d_ptr->handleFoundNodeFail(current, next_node, current_goal);
			}

			// 更新h值基于当前目标
			next_node->h = d_ptr->calculateHValue(next_node->pos, current_goal);
		}

		++d_ptr->currentStep_;
	}

	d_ptr->clear();

	if (pPaths != CXX_NULLPTR)
	{
		return !pPaths->empty();
	}

	return false;
}