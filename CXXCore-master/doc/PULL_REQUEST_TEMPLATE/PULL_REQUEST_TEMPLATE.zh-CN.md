# 目录

* [代码命名规则](#1)
* [命名规则](#2)
* [中英文命名对照表](#3)
  * [常用操作符与函数](#4)
  * [集合与容器操作](#5)
  * [文件与目录操作](#6)
  * [时间与日期](#7)
  * [系统与事件处理](#8)
  * [数值与算法](#9)
  * [UI元素与界面](#10)
  * [基础类型翻译](#11)

---

<div id="1" ></div>

# 代码命名规则

本规则用于指导炫扩展代码命名和翻译的标准，确保一致性、可读性和可维护性。本文档将命名方式和中英文对照翻译进行系统说明。

<div id="2" ></div>

## 命名规则

- **类名、枚举、结构体**：使用大驼峰命名法。如：`MyClass`, `UserType`。
- **命名空间**：全小写，若有多个单词则使用下划线分隔。如：`my_namespace`。
- **私有变量**：使用小驼峰命名法，并在末尾加 `_` 后缀。如：`myVariable_`。
- **普通函数及变量**：使用小驼峰命名法。如：`myFunction`，`countItems`。
- **排版风格**：使用Allman风格，即花括号单独占一行。
- **禁止省略花括号**：即使在单行代码块中，也必须使用完整的花括号。

<div id="3" ></div>

## 中英文命名对照表

<div id="4" ></div>

### 常用操作符与函数

| 英文                          | 中文别名    |
| ----------------------------- | ----------- |
| `set`                         | 置           |
| `get`                         | 取           |
| `data`                        | 地址         |
| `value`                       | 值           |
| `key`                         | 键           |
| `size`, `count`, `length`     | 大小         |
| `count` (指定条件的数量)       | 数量         |
| `resize`                      | 置大小       |
| `ref`                         | 引用         |
| `close`                       | 关闭         |
| `write`                       | 写           |
| `isValid`                     | 是否有效     |
| `isEmpty`, `empty`, `isNull`  | 是否空       |
| `to`                          | 到           |
| `at`                          | 在           |
| `reset`                       | 重置         |
| `add`                         | 加           |
| `clear`                       | 清空         |
| `remove`                      | 移除         |
| `append`                      | 追加         |
| `prepend`                     | 追加首       |
| `insert`                      | 插入         |
| `replace`                     | 替换         |
| `split`                       | 分割         |
| `fill`                        | 填充         |
| `first`, `front`              | 首           |
| `last`, `back`                | 尾           |
| `last`                        | 最后         |
| `create`                      | 创, 创建     |
| `cancel`, `dismiss`           | 取消         |
| `swap`, `transposed`          | 交换         |

<div id="5" ></div>

### 集合与容器操作

| 英文                          | 中文别名    |
| ----------------------------- | ----------- |
| `begin`                       | 开始         |
| `end`                         | 结束         |
| `cbegin`                      | 常量开始     |
| `cend`                        | 常量结束     |
| `rend`                        | 反向结束     |
| `rcbegin`                     | 反向常量开始 |
| `rcend`                       | 反向常量结束 |
| `push`                        | 压, 压入     |
| `dequeue`                     | 出队         |
| `enqueue`                     | 入队         |
| `top`                         | 顶           |
| `reserve`                     | 预留         |
| `squeeze`                     | 收缩         |
| `take`                        | 取出         |
| `indexOf`                     | 找索引       |
| `all`                         | 所有         |
| `move`                        | 移动         |
| `erase`                       | 擦除         |

<div id="6" ></div>

### 文件与目录操作

| 英文                          | 中文别名    |
| ----------------------------- | ----------- |
| `path`                        | 路径         |
| `dir`, `directory`            | 目录         |
| `absolute`                    | 绝对         |
| `file`                        | 文件         |
| `open`                        | 打开         |
| `close`                       | 关闭         |
| `save`, `store`               | 存           |
| `load`, `read`                | 读           |
| `write`                       | 写           |
| `remove`                      | 移除         |
| `flush`                       | 刷新         |
| `line`                        | 行           |
| `extension`                   | 扩展名       |

<div id="7" ></div>

### 时间与日期

| 英文                          | 中文别名    |
| ----------------------------- | ----------- |
| `time`                        | 时间         |
| `date`                        | 日期         |
| `week`                        | 星期         |
| `clock`                       | 时钟         |
| `secs`                        | 秒           |
| `msecs`                       | 毫秒         |
| `timeout`                     | 超时         |
| `pause`                       | 暂停         |
| `resume`                      | 恢复         |
| `stop`                        | 停止         |

<div id="8" ></div>

### 系统与事件处理

| 英文                          | 中文别名    |
| ----------------------------- | ----------- |
| `start`                       | 开始, 启动   |
| `restart`                     | 重启         |
| `process`                     | 处理, 进程   |
| `event`                       | 事件         |
| `invoke`                      | 调用, 方法   |
| `dispatch`                    | 分派         |
| `dispatcher`                  | 分派器       |
| `handle`                      | 句柄         |
| `lock`                        | 锁           |
| `unlock`                      | 解锁         |
| `try`                         | 尝试         |
| `acquire`                     | 占用         |
| `state`, `status`             | 状态         |
| `wait`                        | 等待         |
| `suspend`                     | 挂起         |
| `error`                       | 错误         |
| `debug`                       | 调试         |
| `info`                        | 信息         |
| `message`                     | 消息         |

<div id="9" ></div>

### 数值与算法

| 英文                          | 中文别名    |
| ----------------------------- | ----------- |
| `compare`                     | 比较         |
| `max`                         | 最大         |
| `min`                         | 最小         |
| `total`                       | 总           |
| `code`                        | 码           |
| `hash`                        | 哈希         |
| `sort`                        | 排序         |
| `filter`                      | 过滤器       |
| `calculate`                   | 计算         |
| `value`                       | 值           |
| `length`                      | 长度         |
| `ref`                         | 引用         |

<div id="10" ></div>

### UI元素与界面

| 英文                          | 中文别名    |
| ----------------------------- | ----------- |
| `window`                      | 窗口         |
| `title`                       | 标题         |
| `menu`                        | 菜单         |
| `button`                      | 按钮         |
| `label`                       | 标签         |
| `input`                       | 输入         |
| `output`                      | 输出         |
| `dialog`                      | 对话框       |
| `toolbar`                     | 工具栏       |
| `status`                      | 状态         |
| `box`                         | 框           |

<div id="11" ></div>

## 基础类型翻译

| 英文                          | 中文别名        |
| ----------------------------- | -------------- |
| `bool`                        | 逻辑型         |
| `BYTE`                        | 字节型         |
| `char`                        | 字符型         |
| `wchar_t`, `WCHAR`            | 宽字符型       |
| `short`                       | 短整型         |
| `USHORT`                      | 短正短整型     |
| `int`                         | 整型           |
| `UINT`                        | 正整型         |
| `int64`, `__int64`            | 长整型         |
| `UINT64`                      | 正长整型       |
| `intptr_t`                    | 整数型         |
| `size_t`                      | 正整数型       |
| `long`                        | 长整型32       |
| `ULONG`                       | 正长整型32     |
| `float`                       | 浮点型         |
| `double`                      | 双浮点型       |
| `void`                        | 空类型         |
| `array`                       | 数组           |
| `string`                      | 文本           |

### 文件与系统术语

| 英文                          | 中文别名    |
| ----------------------------- | ----------- |
| `exec`                        | 执行         |
| `run`                         | 运行         |
| `result`                      | 结果         |
| `message`                     | 消息         |
| `generate`                    | 生成         |
| `delete`                      | 删除         |
| `update`                      | 更新         |
| `allocate`, `alloc`           | 申请         |
| `free`                        | 释放         |
| `root`                        | 根           |
| `console`                     | 控制台       |
| `property`                    | 属性         |
| `range`                       | 范围         |
| `timeout`                     | 超时         |
| `callback`                    | 回调         |
| `compare`                     | 比较         |

此规则适用于所有炫扩展代码编写，请严格遵循命名规范以保持代码一致性和高可读性。

---

