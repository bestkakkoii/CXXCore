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
#ifndef CXXASTARDEVICE_P_H_
#define CXXASTARDEVICE_P_H_

#include <cxxastardevice.h>

#include <memory_resource>
#include <functional>
#include <vector>
#include <mutex>
#include <random>

//路径节点状态
enum NodeState
	: __int64
{
	NOTEXIST,               // 不存在
	IN_OPENLIST,            // 在开启列表
	IN_CLOSEDLIST           // 在关闭列表
};

//路径节点
class CXXNode
{
public:
	//计算f值
	inline __int64 f() const
	{
		return g + h;
	}

	inline CXXNode(const CXXPoint& pos)
		: pos(pos)
	{
	}

public:
	__int64 g = 0;                          // 与起点距离
	__int64 h = 0;                          // 与终点距离
	CXXPoint pos;                           // 节点位置
	NodeState state = NodeState::NOTEXIST;  // 节点状态
	CXXNode* parent = CXX_NULLPTR;          // 父节点
};

class CXXAStarDevicePrivate
{
public:
	CXXAStarDevicePrivate(CXXAStarDevice* ptr);
	virtual ~CXXAStarDevicePrivate();

public:
	//清理参数
	void clear();

	//参数是否有效
	bool isValidParams(const CXXPoint& start, const CXXPoint& end) const;

public:
	//二叉堆上滤
	void percolateUp(__int64& hole);

	//获取节点索引
	bool getNodeIndex(CXXNode*& node, __int64* index);

	//计算G值
	__forceinline __int64 calculateGValue(CXXNode*& parent, const CXXPoint& current);

	//计算F值
	__forceinline __int64 calculateHValue(const CXXPoint& current, const CXXPoint& end);

	//节点是否存在于开启列表
	__forceinline bool isInOpenList(const CXXPoint& pos, CXXNode*& out_node);

	//节点是否存在于关闭列表
	__forceinline bool isInCloseList(const CXXPoint& pos);

	//是否可通过
	bool isPassible(const CXXPoint& pos);

	//当前点是否可到达目标点
	bool isPassible(const CXXPoint& current, const CXXPoint& destination, const bool& allow_corner, double& cost);

	//查找附近可通过的节点
	void findPassibleNodes(const CXXPoint& current, const bool& allow_corner, std::vector<CXXPoint>* out_lists);

	//处理找到节点的情况
	void handleFoundNode(CXXNode*& current, CXXNode*& destination);

	//处理未找到节点的情况
	void handleFoundNodeFail(CXXNode*& current, CXXNode*& destination, const CXXPoint& end);

public:
	//路径变异因子
	void setPathVariationFactor(double factor);

	void setStepsBeforeVariation(__int64 steps);

	double getActualCost(const CXXPoint& pos);

public:
	//New method to set alternative goals
	void setAlternativeGoals(const std::vector<CXXPoint>& goals);

	void setDynamicGoalParams(__int64 max_goals, double max_deviation);

	std::vector<CXXPoint> generateAlternativeGoals(const CXXPoint& start, const CXXPoint& end);

public:
	static constexpr __int64 TILE_SIZE = 24;	        // 网格大小
	static constexpr __int64 ORTHOGONAL_COST = 24;      // 直线移动成本
	static constexpr __int64 DIAGONAL_COST = 32;        // 斜角移动成本
	static constexpr __int64 OBSTACLE_PENALTY = 10;     // 障碍物惩罚(额外成本)
	static constexpr __int64 OBSTACLE_CHECK_RADIUS = 2; // 障碍物检查半径
	static constexpr double AVOIDANCE_PENALTY = 2.0;    // 避让惩罚(额外成本)

private:
	//Path node state
	bool					corner_ = true;
	__int64                 height_ = 0;
	__int64                 width_ = 0;
	CXXAStarCallback        can_pass_ = CXX_NULLPTR;
	CXXPoint				start_;
	CXXPoint				end_;
	std::mutex			    mutex_;
	std::vector<CXXNode*>   mapping_;
	std::vector<CXXNode*>   open_list_;
	std::vector<CXXNode*>   record_;

	//Memory resource
	std::unique_ptr<std::pmr::monotonic_buffer_resource> resource_;
	std::unique_ptr<std::pmr::polymorphic_allocator<CXXNode>> allocator_;

	//Random path variation
	std::mt19937 rng_;
	std::uniform_real_distribution<double> dist_;
	double pathVariationFactor_;
	__int64 stepsBeforeVariation_;
	__int64 currentStep_;

	//Path variations
	std::vector<CXXPoint> alternativeGoals_;
	__int64 maxAlternativeGoals_;
	double maxDeviationFactor_;
private:
	CXX_DECLARE_PUBLIC(CXXAStarDevice);
	CXXAStarDevice* q_ptr = nullptr;
};

#endif // CXXASTARDEVICE_P_H_