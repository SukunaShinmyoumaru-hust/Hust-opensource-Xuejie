# 绪论

数据是描述事物的符号记录，是数据库中存储的基本对象

数据的含义称为数据的*语义*

```
学校人事档案中的职工记录
（0005796,  201,  尤政,  1,  1963.11.10,  01 ）
校长办公室
男
▪ 语义：工号, 部门编号, 姓名, 性别, 出生日期, 民族
```

数据库（Database, 简称DB）是长期储存在计算机内、 有组织的、可共享的大量数据的集合

**数据库的特征：**

1. 数据按一定的数据模型组织、描述和存储
2. 可为用户共享
3. 冗余度小
4. 数据独立性高
5. 易于拓展

数据库管理系统是位于用户与操作系统之间的一层数据管理软件，科学地组织和存储数据、高效地获取和维护数据

DBMS的功能：

数据定义功能 ：

• 提供数据定义语言(DDL) ：定义数据库中的数据对象（类型，约束条件等）

▪ 数据组织、存储和管理：

​            分类组织、存储和管理各种数据 • 确定组织数据的**文件结构和存取方式** • 实现数据之间的联系 • 提供多种存      取方法提高存取效率

▪ 数据操纵功能

​           提供数据操纵语言(DML) • 实现对数据库的基本操作  (查询、插入、删除和修改)

 ▪ 数据库的事务管理和运行管理

​           数据并发使用，安全性，完整性和数据系统恢复

▪ 数据库的建立和维护功能(实用程序)

数据库系统：在计算机系统中引入数据库后的系统

包括数据库，数据库管理系统，应用系统和数据库管理员

数据管理： 对数据进行分类、组织、编码、存储、检索和维护

文件系统数据库：数据的共享性差，冗余度大；数据与程序的独立性差，数据的逻辑结构改变则必须修改应用程序

##  数据库系统的特点

数据结构化，数据的共享性高，冗余度低，易扩充，数据独立性高，数据由DBMS统一管理和控制

### 数据结构化

不在针对某一个具体应用，而是面对全组织，整体结构化，内部数据结构化，数据之间有联系

用数据模型描述无需程序定义解释；可以变长；**最小的存取单位是数据项**

### 数据的共享性高，冗余度低，易扩充

从整体角度看待和描述数据，数据 面向整个系统，可以被多个用户、多个应用共 享使用

### 数据独立性高（数据和程序）

物理独立性（存储变了，另外一个不改变） ，逻辑独立性（逻辑结构变量，另外一个不改变）

### 数据由DBMS统一管理和控制

安全性，完整性，并发性，回恢复

### 数据库系统的功能特性

数据独立，数据安全，可控冗余，数据共享，数据完整，数据一致，并发控制，集中存放，故障恢复，数据字典

## E-R图

### 实体型

矩形表示。内部写实体名

### 属性

椭圆形表示，并用无向边将其与相应的实体连接起来

<img src="C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612103656153.png" alt="image-20250612103656153" style="zoom:200%;" />

### 联系

用菱形表示，菱形框内写明联系名，并用无向边分 别与有关实体连接起来，同时在无向边旁标上联系 的类型（1:1、1:n或m:n） 

![image-20250612104016539](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612104016539.png)

### 两个以上实体型之间的多对多联系

![image-20250612104206153](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612104206153.png)

❖**联系的属性**： 联系本身也是一种实体型，也    可以有属性。如果一个联系具 有属性，则这些属性也要用无 向边与该联系连接起来

![image-20250612104553410](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612104553410.png)

#### 主码

用下划线 来标明实体型的主码属性，主码可能是 单个属性，也可能是多个属性构成的属性集。

![image-20250612104700902](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612104700902.png)

## 模型

#### 非关系模型

层次模型是数据库系统中最早出现的数据模型，用树形结构来表示各类实体以及实体间的联系  

网状模型

#### 关系模型

在用户观点下，关系模型中数据的逻辑结构是一张二维表，它由行和 列组成。

一张表是一个关系，一个行使一个元组，一个列式一个数下

主码：**表中的某个属性组，它可以唯一确定一个元组**

域：**属性的取值范围**

分量：**元组中的一个属性值**

关系模式：**关系名（属性1，属性2，…，属性n） 学生（学号，姓名，年龄，性别，系，年级）**

#### 规范化的关系

满足一定的规范条件：**关系的每一个分量必须是一个不可分的数据项, 不允许表中还有表 **

**一个数据项是一个元组 可拆分的数据项就是表里面还有表**

非规范关系：表中有表（大表中嵌有小表）

#### 关系的完整性约束条件

实体完整性 ，参照完整性，用户定义的完整性

#### 存储结果

实体及实体间的联系都用表来表示，表以文件形式存储

### 数据库系统模式

#### 型（Type 模式（Schema）

对某一类数据的结构和属性的说明

#### 值（value）实例（Instance）

是型的一个具体赋值

学生记录型： （学号，姓名，性别，系别，年龄，籍贯） 一个记录值： （900201，李明，男，计算机，22，江苏）

### 数据库模式

❖模式（也称逻辑模式） ▪ 数据库中全体数据的**逻辑结构和特征**的描述 ▪ 所有用户的公共数据视图，综合了所有用户的需求

❖外模式（也称子模式或用户模式） ▪ **数据库用户**（包括应用程序员和最终用户）使用的局部数 据的逻辑结构和特征的描述（不同的用户对数据库的权限不同 是模式的子集）

❖内模式（也称存储模式） ▪ 是数据物理结构和存储方式的描述 ▪ 是数据在数据库内部的表示方式（一个数据库只有一个内模式）

### 数据库的二级映像功能与数据独立性

二级映象在DBMS内部实现这三个抽象层次的联系和转换 

▪ 外模式／模式映像 

▪ 模式／内模式映像

#### 一、外模式／模式映象

定义外模式与模式之间的对应关系，映象定义通常 包含在各自外模式的描述中。

模式改变时，数据库管理员修改有关的外模式／模 式映象，使外模式保持不变

（只修改映像 不修改外部呈现的内容 也就是说 当我们增加用户和权限的时候 给某一个已经有权限的用户的权限不会改变）

应用程序是依据数据的外模式编写的，从而应用程序 不必修改，保证了数据与程序**的逻辑独立性，简称数 据的逻辑独立性**

#### 二、模式／内模式映象

当数据库的存储结构改变了（例如选用了另一种存储 结构），数据库管理员修改模式／内模式映象，使模 式保持不变，从而使应用程序不受影响，保证了数据 与程序的物理独立性，简称数据的物理独立性

（无论怎么改存储 都会适配dml）

# 关系数据库

关系代数是用对关系的运算来表达查询要求的方式。

•关系演算是用谓词表达查询要求的方式

## 笛卡尔积（Cartesian Product） 

▪ *一组*域D1 , D2 ,…, Dn的笛卡尔积为 D1×D2×…×Dn = {(d1 , d2 , … , dn) | di∈Di , i=1,…,n}，笛卡尔积的每个元素叫做一个n元组，元组的每一个值叫做元组的一个**分量**

![image-20250612143506690](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612143506690.png)

笛卡尔积关系的性质：列和行的顺序没关系，但是名字不能相同，元组不能完全相同，某一个分量（列）不可再分（**不能有子表**）

## 例子

![image-20250612143859949](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612143859949.png)

候选码：：在一张数据表中，能够**唯一标识**每一行记录的任意一个字段或字段组合，并且这个组合是**最小的**（即去掉其中任何一个字段，都不能再唯一标识记录），就被称为候选码

**唯一性 (Uniqueness)**：表中任意两行记录的候选码值都不能相同。

**最小性 (Minimality)**：不能再分。例如，`{学号, 姓名}` 不是一个好的候选码，因为仅 `学号` 就足以唯一标识学生，`姓名` 是多余的。

任何一个候选码中的属性叫主属性

主码：主码是从所有**候选码**中**挑选出来的一个**，作为这张表的主要标识符。主码是数据库设计者选定的、用于在表中唯一标识每一条记录的那个字段或字段组合

它必须是候选码（满足唯一性和最小性）。

一张表**只能有一个**主码。

主码的值**绝不能为 NULL**（空值）。

外码：外码是用于**建立和加强两张表之间联系**的字段或字段组合。一张表中的外码，指向的是**另一张表的主码**。

## 关系模式

关系的描述称作关系模式，包括关系名、关系中 的属性名、属性向域的映象、属性间的数据依赖 关系等，记作R(A1 , A2 ,…, An ) **表的属性**

某一时刻对应某个关系模式的内容(元组的集合)称 作关系**具体哪一张表**

## 关系数据库

 其型是关系模式的集合，即数据库描述，称作数据 库的内涵(Intension)，或关系数据库模式

其值是某一时刻关系的集合，称作数据库的外延 (Extension)，或关系数据库

## 实体完整性

主码属性不能为空值

## 参照完整性

对于关系A，他的属性a如果是B的b属性的外码 那么a属性要么是b的一个值，要么为空值

 ## 用户定义的完整性

 • 用户针对具体的应用环境定义的完整性约束条件 • 如S#要求是8位整数，SEX要求取值为“男”或“女”

## 关系代数

### 并运算

所有至少出现在两个关系中之一的元组集合 R  S ={ r | r  R   r  S 

**两个关系的列数量和列名一定一样 而且列的域一定一样**

![image-20250612151302089](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612151302089.png)

### 差运算

所有出现在一个关系而不出现在另外一个关系的元组

**两个关系的列数量和列名一定一样 而且列的域一定一样**

![image-20250612151402486](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612151402486.png)

### 交运算

同时出现在两个关系的元组的集合

R  S ={ r | r RS  R   r  S }

 **差重写：RS = R − (R − S)**

![image-20250612151529004](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612151529004.png)

### 笛卡尔积

![image-20250612152428144](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612152428144.png)

### 分量 属性列和象集

![image-20250612152657326](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612152657326.png)

![image-20250612161017794](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612161017794.png)

### 选择

![image-20250612161645207](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612161645207.png)

#### 格式：

![image-20250612161913116](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612161913116.png)

### 投影

![image-20250612162359249](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612162359249.png)

#### 格式：

![image-20250612162730714](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612162730714.png)

### 链接

![image-20250612162922981](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612162922981.png)

**先笛卡尔后选 **

![image-20250612163548953](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612163548953.png)

![image-20250612163326723](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612163326723.png)

**当自然链接之间有相同的属性列的时候，回按照相同列匹配链接**

### 外连接

在自然链接的基础上如果把药舍弃的元组也保留到结果里面，在这个元组的其他属性上填写空值，这叫外连接

### 左外链接

左边关系R里面的药舍弃的元组保留

### 右外连接

右边关系S里面的药舍弃的元组保留

![image-20250612165316719](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612165316719.png)

### 除法：

说人话 就是两个关系里面有n个**域**的含义一模一样的属性组 他们名字可以不一样 除法的结果就是这几个属性组**完全匹配之后** R剩下的属性组的值 也就是说R里面有一个元组X 他的Y有多个 全部和S匹配

![image-20250612170052352](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612170052352.png)

![image-20250612170201840](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612170201840.png)

![image-20250612170212379](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612170212379.png)

![image-20250612170556023](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612170556023.png)

# SQl语句

特点

1. 综合统一，ddl，dml，dcl多个合一
2. 高度非过程化
3. 面向集合的操作方式
4. 以同一种语法结构提供多种使用方式
5. 语言简洁，易学易用

```
SELECT a
 FROM R, S
 WHERE R.b = S.b;
```

基础语句 阅读顺序先from后where再select

```
SELECT a
 FROM R, S
 WHERE b in (select b from s)
```

in 语句表示二重查询

## SQL支持关系数据库三级模式结构

![image-20250612183925838](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612183925838.png)

基本表 ◼ 本身独立存在的表 ◼ SQL中一个关系就对应一个基本表 ◼ 一个(或多个)基本表对应一个存储文件 ◼ 一个表可以带若干索引 

❖存储文件 ◼ 逻辑结构组成了关系数据库的内模式 ◼ 物理结构是任意的，对用户透明

 ❖视图 ◼ 从一个或几个基本表导出的表 ◼ 数据库中只存放视图的定义而不存放视图对应的数据 ◼ 视图是一个虚表 ◼ 用户可以在视图上再定义视图

## 数据定义

![image-20250612185157034](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612185157034.png)

#### 模式定义

```
CREATE SCHEMA “S-T” AUTHORIZATION WANG; //给用户wang定义了一个名叫s-t的模式
CREATE SCHEMA AUTHORIZATION WANG； //没有制定模式名那么模式名隐含为用户名

```

定义模式实际上定义了一个命名空间 ❖在这个空间中可以定义该模式包含的数据库对象，例如**基本表、 视图、索引**等

```
CREATE SCHEMA <模式名> AUTHORIZATION <用户名>[<
表定义子句>|<视图定义子句>|<授权定义子句>]
```

```
 CREATE SCHEMA TEST AUTHORIZATION ZHANG 
CREATE TABLE TAB1(COL1 SMALLINT， 
                                            COL2 INT，
                                            COL3 CHAR(20)，
                                            COL4 NUMERIC(10，3)，
                                            COL5 DECIMAL(5，2)
 )；
    为用户ZHANG创建了一个模式TEST，并在其中定义了一
个表TAB1。

```

#### 模式删除

```
DROP SCHEMA<模式名> <CASCADE | RESTRICT>
```

 CASCADE(级联)： 删除模式的同时把该模式中所有的数据库对象全部删除

 RESTRICT(限制)： 如果该模式中定义了下属的数据库对象（如表、视图等），则拒绝 该删除语句的执行。 当该模式中没有任何下属的对象时才能执行。

#### 表的定义

```
CREATE TABLE <表名>
（<列名><数据类型>[<列级完整性约束条件>]
 [，<列名><数据类型>[<列级完整性约束条件>]]…
 [，<表级完整性约束条件>]）；
```

如果完整性约束条件涉及到该表的多个属性列， 则必须定义在表级上，否则既可以定义在列级 也可以定义在表级。

建立“学生”表Student，学号是主码，姓名取值唯一。

```
CREATE TABLE Student          
(Sno   CHAR(9) PRIMARY KEY， /* 列级完整性约束条件*/                  
Sname  CHAR(20) UNIQUE，     /* Sname取唯一值*/
 Ssex    CHAR(2)，
Sage   SMALLINT，
Sdept  CHAR(20)
 )；
 CREATE TABLE  Course
 ( Cno      
 CHAR(4) PRIMARY KEY，
Cname  CHAR(40)，            
Cpno     CHAR(4) ，                                     
Ccredit  SMALLINT，
FOREIGN KEY (Cpno) REFERENCES  Course(Cno)
 ); //foriengh key后面接一个括号表示约束的外码是什么 references表示约束的条件是什么
```

单个主码可以在列描述，多个主码只能在表级一起描述，多个外码只能一个个在表级描述

## 数据类型

定义表的属性时需要指明其数据类型及长度 ，在属性的后面直接跟属性类型名字

![image-20250612193415592](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250612193415592.png)

## 模式和表

一个表属于某一个模式

一个模式包含多个表

定义表和模式（**.语句表示归属**）

方法一：在表名中明显地给出模式名  Create table “S-T”.Student（......）;   /*模式名为 S-T*/ Create table “S-T”.Cource（......）; Create table “S-T”.SC（......）;  

◼ 方法二：在创建模式语句中同时创建表  

◼ 方法三：设置所属的模式 

###  如果没有指定表的模式

系统会搜索路径找到第一个存在的模式作为数据库对象的模式名字

如果**指定了模式但是模式不存在**

显示当前搜索路径：

```
 SHOW search_path; 
```

默认：

```
$user， PUBLIC
```

可以先设置搜索路径，在定义基本表，这样就会建立一个搜索路径下的表。使用时rdbms发现搜索路径里面第一个模式名s-t存在，就把该模式作为表所属模式

```
SET search_path TO “S-T”，PUBLIC；
Create table Student（......）;  
```

#
## 修改表

```
ALTER TABLE <表名>
 [ ADD <新列名><数据类型>[完整性约束 ] ]
 [ DROP <完整性约束名>]
 [ ALTER COLUMN<列名> <数据类型>]；
```

### 加入新列 注意要写列的数据类型

```
ALTER TABLE Student ADD S_entrance DATE；
不论基本表中原来是否已有数据，新增加的列一律为空值。
```

### 修改列的数据类型：两个alter

```
ALTER TABLE Student ALTER COLUMN Sage INT；
```

### 增加约束条件

```
ALTER TABLE Course ADD UNIQUE(Cname);
```

### 删除基本表

```
DROP TABLE <表名>［RESTRICT | CASCADE］；
```

#### ◼CASCADE：删除该表没有限制

在删除基本表的同时，相关的依赖对象一起删除 

#### ◼RESTRICT：删除表是有限制的(缺省)。

欲删除的基本表不能被其他表的约束所引用,如果有依赖这个表的对象，这个表无法被删除

**上面这个对象指的是这个表上面索引视图和触发器之类的**

```
DROP TABLE  Student  CASCADE 
```

 ◼基本表定义被删除，数据被删除 ◼表上建立的索引、视图、触发器等也将被删除 

## 索引

数据库管理员和数据库表建立人可以建立索引，数据库一般会自动建立以下列上的索引

**❖RDBMS中索引一般采用B+树、HASH索引来实现**

### sql建立索引语句：CREATE INDEX语句 定义索引是唯一 索引、非唯一索引或聚簇索引 

```
CREATE [UNIQUE] [CLUSTER] INDEX <索引名>
 ON<表名>(<列名>[<次序>][,<列名>[<次序>] ]…)；
```

#### UNIQUE：

**唯一性索引**，不允许表中不同的行在索引列上取相 同值。若已有相同值存在，则系统给出相关信息，不建此索 引。

#### CLUSTER：

聚集索引，表中元组按索引项的值排序并物理地 聚集在一起。

#### 次序

次序（ASC/DESC）：索引表中索引值的排序次序，缺省为 ASC。

#### 例子

```
CREATE CLUSTER INDEX Stusname  ON   Student(Sname)；
```

在Student表的Sname（姓名）列上建立一个聚簇索引

在最经常查询的列上建立聚簇索引以提高查询效率

一个基本表上最多只能建立一个聚簇索引 

```
CREATE UNIQUE INDEX Stusno ON Student(Sno)；
CREATE UNIQUE INDEX Coucno ON Course(Cno)；
CREATE UNIQUE INDEX  SCno ON SC(Sno ASC，Cno 
DESC)；
```

### 删除索引

```
DROP INDEX <索引名>；
DROP INDEX Stusname；
```

**不允许用户在数据操作中引用索引，索引如何使用完 全由系统决定**

## 数据库的建立

```
create  database 数据库名

drop  database  数据库名

指定当前数据库:
database 数据库名
use 数据库名
```

## 查询操作

![image-20250613160649010](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250613160649010.png)

可以在查询之间使用 UNION 运算符，以将查询的结果组合 成单个结果集这句话的意思是，**在两个列名一样的在行上链接**

### 基本查询

```
SELECT A1 , A2 , … , An
 FROM R1,R2, …, Rm
 WHEREP
 SELECT *    FROM Dept //*选择全部
```

![image-20250613161415467](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250613161415467.png)

先交叉积 按照p的条件选择an π符号取出的是列

#### 支持算数表达式

```
SELECT pname，sal*0.9
 FROM Prof
```

#### 支持聚合函数**avg，max，min**

```
SELECT AVG(sal) FROM Prof
```

#### 重复元组处理

**缺省**为保留重复元组，也可用关键字ALL显式指 明

若要去掉重复元组，可用关键字DISTINCT指明

```
SELECT DISTINCT sno 
FROM SC
```

### from 子句

```
SELECT ALL  pname , sal , dname
 FROM Prof , Dept
 WHERE Prof.DNO = Dept.DNO
```

 FROM 子句列出查询的对象表

**同名的列名要加一个前缀**

![image-20250613162513375](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250613162513375.png)

**from语句后面的查询要自然链接**

### where 语句

• 比较运算符 

• 逻辑运算符 AND，OR，NOT

 •**BETWEEN 条件**

​     **判断表达式的值是否在某范围内**

 • IN，LIKE，IS NULL，…

### 简单查询 更名

为关系和属性重新命名，可出现在**SELECT和FROM**子句中，AS可选

```
old_name AS new_name
```

```
SELECT pname，sal0.05  AS tax，
sal * 0.95  AS incoming
 FROM Prof
```

### 字符操作匹配

```
列名  [NOT]  LIKE ‘<匹配串>’ [ESCAPE ‘<换码字符>’
```

匹配规则：

“%” ：匹配零个或多个字符

 • “_” ：匹配任意单个字符 _

_• ESCAPE ：**定义转义字符，以去掉特殊字符的特定含义， 使其被作为普通字符看待 _**

​         如ESCAPE“\”，定义 \作为转义字符，则可用\%去 匹配%，用\_去匹配_

```
SELECT * 
FROM Prof
 WHEREpname LIKE ‘张%’
 
 SELECT sname
 FROM S
 WHEREsname LIKE ‘司马_ _ _ _’
```

 Ex 3: 列出名称中含有4个字符以上，且倒数第3个字符是d， 倒数第2个字符是_的系的所有信息

```
SELECT *
 FROM Prof
 WHERE  pname  LIKE ‘% _d \_ _’ ESCAPE ‘\’
```

### 简单查询 顺序查询

```
ORDERBY 列名   [ASC
 | DESC
```

 按系名升序列出老师姓名，所在系名，同一系中老 师按姓名降序排列

```
SELECT dname,pname
 FROM Prof,Dept
 WHERE Prof.d# = Dept.d#
 ORDERBY dname ASC，pname DESC
```

### 聚集函数

```
• COUNT([DISTINCT] A) / COUNT([DISTINCT]*)
 • SUM ([DISTINCT] A)
 • AVG ([DISTINCT] A)
 • MAX (A)
 • MIN (A)
```

distint唯一指代

### 分组

```
GROUP  BY 列名  [HAVING 条件表达式]
```

**group分组 然后在每一个组上面用聚合函数 有多少个组最后的结果就有多少行 having为分组条件 只会对大于这个条件的行分组**

### 空值

测试是不是空值

```
IS [NOT]   NULL
```

除is [not] null之外，空值不满足任何查找条件

– 如果null参与算术运算，则该算术表达式的值为null

– 如果null参与比较运算，则结果可视为false，在SQL 92中可看成unknown

– 如果null参与聚集运算，则除count(*)之外其它聚集函 数都忽略null

**null属于 unknown类型数 null可以被用来赋值 任何值（包括 NULL 本身）与 NULL 进行比较，结果都是 未知 (UNKNOWN) 当且仅当 WHERE 子句的条件为 真 (TRUE) 时，一条元组（行）才会被包含在查询结果中**

## 嵌套查询

 子查询是嵌套在另一查询中的 Select-From-Where 表达式， 子查询中不能使用 Order By 子句，Order By子句只能对最 终查询结果进行排序

### in子句 集合成员查询

```
表达式  [ NOT]  
IN （子查询）–  判断表达式的值是否在子查询的结果中
```

```
select *
 from S
 where SNAME  in 
（“张军”,“王红”）
```

```
select SNO，SNAME
 from S
 where SNO   in
 (select SNO
 from SC
 where CNO = 001）
```

**例子**

```
Student(sno, sname, ssex, sage, sdept)
 Course(cno, cname, cpno, ccredit)
 SC(sno, cno, grade)
例: 查询选修了课程名为‘信息系统’的学生学号和姓名。
```

**取中介表作为媒介**

```
SELECT  Sno，Sname        
FROM  Student
 WHERE  Sno IN
 (SELECT SnO
 FROM  SC
 WHERE Cno IN
 (SELECT Cno
 WHERE Cname＝ ‘信息系统’))
```

### 集合的比较

 带有比较运算符的子查询，当用户能确切知道内层查询返回的是**单列**时，可 以用>、<、＝、>＝、<＝、!＝或<>等比较运 算符。  

```
Student(sno, sname, ssex, sage, sdept)
 Course(cno, cname, cpno, ccredit)
 SC(sno, cno, grade)
```

```
SELECT sname
 FROMStudent
 WHERESdept = ( SELECT sdept
 FROMStudent
 WHERE sname = ‘王红’) 
```

### any和all

一个匹配就ok：

```
表达式   比较运算符   ANY （子查询）
```

 表达式的值至少与子查询结果中的一个值相比满足 比较运算符

全部匹配

```
表达式   比较运算符  ALL（子查询）
```

#### 没有all in的用法

```
= all 是是错的
```

```
Student(sno, sname, ssex, sage, sdept)
 Course(cno, cname, cpno, ccredit)
 SC(sno, cno, grade)
例:  查询其他系中比IS系所有学生年龄都小的学生名单。
```

```
SELECT Sname，Sage
 FROM Student
 WHERE Sage<ALL
 (SELECT Sage
 FROM Student
 WHERE Sdept='IS')
 AND Sdept<>’IS’
 ORDER BY Sage DESC；
```

### exist

相关嵌套查询(EXISTS)

```
[NOT] EXISTS （子查询）
```

 Exists + 子查询可用以测试该子查询的结果是否有元组,带有Exists的子查询不返回任何数据，只产生  True/False

```
 Student(sno, sname, ssex, sage, sdept)
 Course(cno, cname, cpno, ccredit)
 SC(sno, cno, grade)
```

```
SELECT Sname
 FROM Student
 WHERE EXISTS  ( SELECT  *
 FROM SC
 WHERE Sno=Student.Sno AND Cno=‘1’)；
```

**相当于先进行子查询，先读取sno取比较，用这个值去检查sc关系，若sc里面有这种元组，用这种适合的sname送入关系**

```
IN后的子查询与外层查询无关，每个子
查询执行一次，而EXISTS后的子查询与外层
查询有关，需要执行多次。
```

IN后的子查询与外层查询无关，每个子 查询执行一次，而EXISTS后的子查询与外层 查询有关，需要执行多次。

### 集合查询

◼集合并：union ◼集合交：intersect ◼集合差：except

**集合操作自动去除重复元组，如果要保留重复元组 的话，必须用all关键词指明**

```
(select
 from
 SNO
 SC
 where CNO = 001)
 union (intersect)
 (select
 SNO
 from
 SC
 where CNO = 002) //选了1和2的
```

```
(select
 from
 SNO
 SC
 where CNO = 001  or CNO = 002 )
 except
 (select
 from
 SNO
 SC
 where CNO = 003) //选了1和2没有3的
```

## 数据更新

### 插入数据

#### 插入单元组

```
INSERT  INTO 表名  [（列名[，列名]…]
 VALUES (值 [，值]…
```

```
INSERT INTO Prof
 VALUES ( ‘P0011’, ‘王明’, 35, 08, 4980 )
 INSERT INTO Prof (P#, PNAME, D#)
 VALUES ( ‘P0012’, ‘李明’, 03 )
```

#### 插入子查询

```
INSERT INTO 表名  [（列名[，列名]…]
（子查询）
```

```
 INSERT INTO Excellent ( s#, avg_score)
 SELECT s#,AVG(score)
 FROM SC
 GROUPBYs#
 HAVING AVG(score) > 90
```

### 删除数据

```
DELETE FROM <表名>
 [WHERE <条件表达式>]
从表中删除符合条件的元组，如果没有where语句，
则删除所有元组
```

```
DELETE FROMSC
```

```
DELETE
 FROM Student
 WHERE Sno＝‘95019’
```

### 更新数据

```
UPDATE 表名  
SET 列名 = 表达式 | 子查询
列名 = [，表达式 | 子查询]…
 [WHERE 条件表达式]
```

```
 UPDATE Prof
 SET sal = sal * 1.05
  将教师的工资上调5%
```

#### 结合集合比较符

 将01系系主任的工资改为该系的平均工资

```
UPDATE Prof
 SET sal = (SELECT AVG(sal)
 FROM Prof
 WHEREd#=‘01’)
 WHEREpname=(SELECT dean
 FROM Dept
 WHEREd#=‘01’)
```

## 视图

❖虚表，是从一个或几个基本表（或视图）导出的表

![image-20250615101445718](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250615101445718.png)

**只存放一个窗口，一个定义，不放数据 这意味着不存放图的数据，这意味着如果基表变了 视图也会变 这也意味着更新难以进行，但是可以查询**

```
CREATE [MATERIALIZED] VIEW <视图名> AS <查询
表达式>  [WITH CHECK OPTION]
```

**`<查询表达式>`**: 定义这个视图的 `SELECT` 查询语句,也就是窗口大小

#### 物化视图

与普通视图最大的不同在于，它**真的会把查询结果存储在物理磁盘上**。它是一个真实的数据快照。

- **为什么要用？** 为了**性能** 🚀。如果一个视图的定义查询非常复杂、运行很慢（比如对几百万行数据进行分组聚合），那么每次都重新计算会非常低效。物化视图预先计算好结果并存起来，查询它就会变得飞快。
- **代价是什么？** 存储的数据可能会**过时**。如果原始表（`员工`表）的数据变了，物化视图的内容不会自动更新，你需要手动“刷新”（REFRESH）它。**相当于重定义了一个图 但是有链接 可以手动刷新**

#### 更名

视图可以改名字 列也可以改名字

```
-- 这里我们显式地将列命名为 "全名" 和 "所属部门"
CREATE VIEW 员工名录 (全名, 所属部门) AS
SELECT 姓名, 部门
FROM 员工;
```

####  WITH CHECK OPTION - 检查选项

这是一个非常重要的约束，用于保证通过视图进行的 `INSERT` 和 `UPDATE` 操作的数据完整性。

- **它的作用是**: 当你通过一个带 `WITH CHECK OPTION` 的视图去**更新**或**插入**数据时，数据库会检查这条新数据或修改后的数据**是否满足视图定义中的 `WHERE` 条件**。如果不满足，操作将被拒绝。

**例子**： 我们为“市场部”创建一个带检查选项的视图。

SQL

```
CREATE VIEW 市场团队_检查 AS
SELECT 员工ID, 姓名, 部门
FROM 员工
WHERE 部门 = '市场部'
WITH CHECK OPTION;
```

现在看看它的效果：

- 成功的更新

  : 修改李四的名字是没问题的，因为他仍然属于“市场部”。

  SQL

  ```
  -- 这个操作会成功
  UPDATE 市场团队_检查 SET 姓名 = '李武' WHERE 员工ID = 102;
  ```

- 失败的更新

  : 如果你想通过这个视图把李四从“市场部”调到“销售部”，操作会

  失败

   ❌。因为修改后的数据 (

  ```
  部门 = '销售部'
  ```

  ) 不再满足视图的 

  ```
  WHERE 部门 = '市场部'
  ```

   条件，

  ```
  WITH CHECK OPTION
  ```

   会阻止这个操作。

  SQL

  ```
  -- 这个操作会被拒绝
  UPDATE 市场团队_检查 SET 部门 = '销售部' WHERE 员工ID = 102;
  ```

这个选项确保了用户不能通过视图添加一条他们自己在这个视图里都看不到的数据。

**可以理解为窗口看不到那个值 无法这样修改**

#### 限制: `ORDER BY` 和 `DISTINCT`

限制: `ORDER BY` 和 `DISTINCT`

### 基于多个表的视图

```
create view S_detail  as 
select sno, sname, cname, grade
 from student, sc, course
 where student.sno = sc.sno  and
 course.cno = sc.cno 
```

### 删除视图

```
DROP  VIEW <视图名>；
```

，使用CASCADE 级联删除语句，把该视图和由它导出的所有视图一 起删除 

删除基表时，由该基表导出的所有视图定义都**必须 显式**地使用DROP VIEW语句删除 

```
错误：
SELECT Sno，AVG(Grade)
 FROM     SC
 WHERE  AVG(Grade)>=90
 GROUP BY Sno；
正确：
SELECT  Sno，AVG(Grade)
 FROM  SC
 GROUP BY Sno
 HAVING AVG(Grade)>=90；
```

上面那个是会先选择大于90的，剔除调不是90的 再进行分组

下面是分为两组 一组条件是大于90一组小于90

### 更新视图

 对视图的更新，最终要转换为对基本表的更新

```
 CREATE VIEW IS_Student  AS
 SELECT Sno, Sname, Sage 
FROM Student 
WHERE Sdept='IS‘
 WITH CHECK OPTION;
```

▪ SELECT子句中的目标列不能包含聚集函数 

▪ SELECT子句中不能使用UNIQUE或DISTINCT关键字 

▪ 不能包括GROUP BY子句 

▪ 不能包括**经算术表达式计算出来的列**(视图只是窗口)

▪ 对于行列子集视图可以更新（视图是**从单个基本表使用 选择、投影操作导出的**，**并且包含了基本表的主码**）

# 安全

## 授权

```
GRANT <权限>[,<权限>]... 
ON <对象类型> <对象名>[, <对象类型> <对象名>]…
 TO <用户>[,<用户>|PUBLIC]...
 [WITH GRANT OPTION];
```

![image-20250615105125928](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250615105125928.png)

**PUBLIC（全体用户）**

发出的人：DBA ➢ 数据库对象创建者（即属主Owner） ➢ 拥有该权限的用户

```
Ex 1:  把查询Student表权限授给用户A
 GRANT SELECT ON TABLE Student TO A;
 Ex2: 把对Student表和Course表的全部权限授予用户U2和U3
 GRANT ALL PRIVILIGES ON TABLE Student, Course TO U2, U3; 
Ex3: 把对表SC的查询权限授予所有用户
GRANT SELECT ON TABLE SC TO PUBLIC;
Ex 4: 把查询SC表和修改学生成绩的权限授给用户U4
 GRANT UPDATE(Score), SELECT 
ON TABLE SC 
TO U4;
注：对属性列的授权时必须明确指出相应属性列名
Ex 5:  把对表SC的INSERT权限授予U5用户，并允许他再将此权
限授予其他用户
GRANT INSERT ON TABLE SC TO U5
 WITH GRANT OPTION;
```

## 收回

```
 REVOKE <权限>[,<权限>]... 
ON <对象类型> <对象名>[, <对象类型> <对象名>]… 
FROM <用户>[,<用户>| public] … 
[CASCADE | RESTRICT]…
 CASCADE：收回权限时，若该用户已将权限授予其它用户，则也一
并收回；否则，拒绝执行（RESTRICT）
```

```
REVOKE UPDATE(sno) ON TABLE Student FROM U4
```

DBA：拥有所有对象的所有权限  不同的权限授予不同的用户 ◼ 用户：拥有自己建立的对象的全部的操作权限  GRANT：授予其他用户 ◼ 被授权的用户  “继续授权”许可：再授予 ◼ 所有授予出去的权力在必要时又都可用REVOKE语句收回

## 创建数据库模式的权限

dba在创建用户的时候给予（不然用户都没办法创建数据库）

```
CREATE  USER  <username> 
［WITH］［DBA | RESOURCE | CONNECT］
```

![image-20250615142834939](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250615142834939.png)

 最后一个connect的意思是必须有其他人给他的权限 他需要别人授权

### 数据库角色

就是权限的集合 一个人有多个权限，可以为一组有相同权限的用户创建一个角色

#### 角色的创建

```
CREATE  ROLE  <角色名> 
```

#### 给角色授权

```
GRANT  <权限>［，<权限>］… 
ON <对象类型><对象名>
 TO <角色>［，<角色>］…
```

####  讲一个角色授予其他用户or角色

```
GRANT  <角色1>［，<角色2>］…
 TO  <角色3>［，<用户1>］…
 ［WITH ADMIN OPTION]
```

**［WITH ADMIN OPTION］ 获得了某种权限的角色或用户还可以将这种权限再授予其他 角色。**

#### 权限收回

```
REVOKE <权限>［，<权限>］…
 ON <对象类型> <对象名>
 27
 FROM <角色>［，<角色>］…

```

#### 创建角色

```
 CREATE  ROLE  R1;
```

#### 授权 注意类型要指明

```
GRANT SELECT，UPDATE，INSERT 
ON TABLE Student 
TO R1;
```

```
GRANT DELETE 
ON TABLE Student
 TO R1;
```

#### 以角色权限授予角色

```
GRANT  R1 
TO 王平, 张明, 赵玲;
```

#### 以角色权限授予角色

```
REVOKE  R1 
FROM 王平;
```

```
 REVOKE SELECT 
ON TABLE Student
 FROM  R1;
```

####  WITH GRANT OPTION：对象权限的传递

这个选项专门用于对象级别的权限，例如对某张表、视图或存储过程的 `SELECT`, `INSERT`, `UPDATE`, `DELETE` 等权限。

- **作用**：允许被授权的用户将他所获得的对象权限**再次授予**给其他用户。
- **权限回收（级联回收 Cascade Revoke）**：这是它的关键特性。如果您收回了用户 A 的权限，那么由 A 授予给用户 B 的所有相关权限也会被**一并自动收回**。这形成了一个权限依赖链。

**场景举例**：

1. 管理员将 

   ```
   scott.emp
   ```

    表的查询权限授予用户 A，并允许他继续授权：

   SQL

   ```
   GRANT SELECT ON scott.emp TO A WITH GRANT OPTION;
   ```

2. 用户 A 现在可以将这个权限授予用户 B：

   SQL

   ```
   GRANT SELECT ON scott.emp TO B; -- A不需要再次指定WITH GRANT OPTION
   ```

3. 收回权限

   ：现在，如果管理员从用户 A那里收回权限：

   SQL

   ```
   REVOKE SELECT ON scott.emp FROM A;
   ```

4. **结果**：用户 B 的查询权限会因为依赖链断裂而**自动失效**。这就是“因传播而失效”的级联效果。

------

#### WITH ADMIN OPTION：系统权限的管理

这个选项专门用于系统级别的权限（如 `CREATE TABLE`, `CREATE SESSION`）和角色。

- **作用**：允许被授权的用户将他所获得的系统权限或角色**管理和授予**给其他用户。
- **权限回收（非级联回收 Non-Cascade Revoke）**：这是它与 `WITH GRANT OPTION` 最根本的区别。如果您收回了用户 A 的权限，用户 B 的权限**不会受到任何影响**。权限一旦授予，就独立存在。

**场景举例**：

1. 管理员将 

   ```
   CREATE TABLE
   ```

    这个系统权限授予用户 A，并允许他管理此权限：

   SQL

   ```
   GRANT CREATE TABLE TO A WITH ADMIN OPTION;
   ```

2. 用户 A 现在可以将这个权限授予用户 B：

   SQL

   ```
   GRANT CREATE TABLE TO B;
   ```

3. 收回权限

   ：现在，如果管理员从用户 A 那里收回权限：

   SQL

   ```
   REVOKE CREATE TABLE FROM A;
   ```

4. **结果**：用户 B 仍然拥有 `CREATE TABLE` 的权限，**完全不受影响**。因为系统权限的授予更像是一种资格认证，一旦认证通过，就与推荐人（授予者）无关了。

#### 视图机制

### 审计

将用户对数据库的所有操作记录在上面

 用户级审计 

➢ 针对自己创建的数据库表或视图进行审计 

➢ 记录所有用户对这些表或视图的一切成功和（或）不成功的访 问要求以及各种类型的SQL操作 

 系统级审计 

➢ DBA设置 

➢ 监测成功或失败的登录要求

 ➢ 监测GRANT和REVOKE操作以及其他数据库级权限下的操作

 ◼ AUDIT语句：设置审计功能 ◼ NOAUDIT语句：取消审计功能

```
［例1］对修改SC表结构或修改SC表数据的操作进行审计
AUDIT ALTER，UPDATE  
ON SC；
［例2］取消对SC表结构或修改SC表数据的操作进行审计
NOAUDIT ALTER，UPDATE  
ON  SC
```

# 完整性

### 实体型约束:主码

```
CREATE  TABLE中用PRIMARY KEY定义
```

单属性构成的码有两种说明方法 定义为列级约束条件 定义为表级约束条件 

◼ 对多个属性构成的码只有一种说明方法 定义为表级约束条件

```
例］将SC表中的Sno，Cno属性组定义为码
CREATE TABLE SC
 (Sno   CHAR(9)  NOT NULL，
Cno  CHAR(4)  NOT NULL，
Grade    SMALLINT，
PRIMARY KEY (Sno，Cno) /*只能在表级定义*/
 );
```

主码的属性不能取Null、也不能有重复值 候选码的属性不能有重复值，可以取Null ◼一般在Primary Key上自动加上Index

### 参照完整性

外码：

在CREATE  TABLE中用FOREIGN KEY定义外码，用 REFERENCES指明这些外码参照哪些表的主码 参照&被参照、主表&从表、主键&外键 外键的值不允许参照不存在的主键的值

◼ 类型 ◼ 属性名可以不同 ◼ 外键允许Null

###  保持参照完整性

各种可能的情况 :◼从表 插入从表元组，且外键不为Null 修改从表外键，且不为Null 

◼主表 删除主表元组，其已被参照 修改主表主键，其已被参照 Drop Table

![image-20250615154541342](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250615154541342.png)

```
［例］显式说明参照完整性的违约处理示例
CREATE TABLE SC
        (Sno   CHAR(9)  NOT NULL，
Cno   CHAR(4)  NOT NULL，
Grade  SMALLINT，
PRIMARY KEY（Sno，Cno），
FOREIGN KEY (Sno) REFERENCES Student(Sno) 
          ON DELETE CASCADE     /*级联删除SC表中相应的元组*/
               ON UPDATE CASCADE，/*级联更新SC表中相应的元组*/
         FOREIGN KEY (Cno) REFERENCES Course(Cno)                     
               ON DELETE NO ACTION 
               /*当删除course 表中的元组造成了与SC表不一致时拒绝删除*/
               ON UPDATE CASCADE   
      /*当更新course表中的cno时，级联更新SC表中相应的元组*/
        )；

```

### 属性上的用户完整性定义

◼ 属性上的约束条件 NOT NULL ◼ Primary Key约束隐含Not Null ◼ 不加约束隐含允许Null UNIQUE ◼ 列值唯一 CHECK ◼ 检查列值是否满足一个布尔表达式

```
CREATE TABLE Student
 (Sno    CHAR(9)，
Sname  CHAR(8) NOT NULL，
Ssex    CHAR(2)，
Sage   SMALLINT，
Sdept  CHAR(20)，
PRIMARY KEY (Sno)，
CHECK (Ssex='女' OR Sname NOT LIKE 'Ms.%')
 /*定义了元组中Sname和Ssex两个属性值之间的约束条件*/
 )
```

### 完整性约束命名

```
CONSTRAINT <完整性约束条件名>
［PRIMARY KEY短语
|FOREIGN KEY短语
|CHECK短语］
```

```
CREATE TABLE Student
 (Sno  NUMERIC(6)
 CONSTRAINT C1 CHECK (Sno BETWEEN 1000 AND 9999)，
Sname  CHAR(20)  
CONSTRAINT C2 NOT NULL，
Sage  NUMERIC(3)
 CONSTRAINT C3 CHECK (Sage < 30)，
Ssex  CHAR(2)
 CONSTRAINT C4 CHECK (Ssex IN ( '男'，'女'))，
CONSTRAINT StudentKey PRIMARY KEY(Sno)
 )；
```

 在Student表上建立了5个约束条件，包括主码约束（命名为 StudentKey）以及C1、C2、C3、C4四个列级约束。

### 修改完整性约束

使用ALTER TABLE语句修改表中的完整性限制

```
ALTER TABLE Student
 DROP CONSTRAINT C1;
 ALTER TABLE Student
 ADD CONSTRAINT C1 CHECK (Sno BETWEEN 1000 AND 
9999)，
ALTER TABLE Student
 DROP CONSTRAINT C3;
 ALTER TABLE Student
 ADD CONSTRAINT C3 CHECK (Sage < 40)；
```

### 断言

每次对数据库中的任何关系进行修改后，都自动检查所有断言。

```
CREATE ASSERTION NoRipoffBars  
CHECK (  
    NOT EXISTS (  
        SELECT bar FROM Sells  
        GROUP BY bar  
        HAVING 5.00 < AVG(price)  
    )  
);
CREATE ASSERTION FewBar  
CHECK (  
    (SELECT COUNT(*) FROM Bars) <=  
    (SELECT COUNT(*) FROM Drinkers)  
);
```

 ###    

### 域中的完整性限制

可以用CREATE DOMAIN语句建立 一个域以及该域应该满足的完整性约束条件。

```
［例］建立一个性别域，并声明性别域的取值范围
CREATE DOMAIN GenderDomain CHAR(2)
 CHECK (VALUE IN ('男'，'女') );
这样［例10］中对Ssex的说明可以改写为
Ssex GenderDomain
［例］建立一个性别域GenderDomain，并对其中的限制命名
CREATE DOMAIN GenderDomain CHAR(2)
 CONSTRAINT GD CHECK ( VALUE IN ('男'，'女') );
 ［例］删除域GenderDomain的限制条件GD。
ALTER  DOMAIN  GenderDomain  
DROP CONSTRAINT GD;
［例］在域GenderDomain上增加限制条件GDD。
ALTER  DOMAIN  GenderDomain  
ADD CONSTRAINT GDD CHECK (VALUE IN ( '1'，'0') ); 
✓ 将性别的取值范围由('男'，'女')改为( '1'，'0')

```

### 触发器

触发器（Trigger）是用户定义在关系表上的一 类由事件驱动的特殊过程,当指定的表上发生特定的操作，系统便激活Trigger 程序

```
CREATE TRIGGER <触发器名>  
{BEFORE | AFTER} <触发事件> ON <表名>
 FOR EACH  {ROW | STATEMENT}
［WHEN <触发条件>］
<触发动作体>
```

![image-20250615160634197](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250615160634197.png)

#### INSTEAD OF触发器

触发器可以实现将**对视图的更新**转换为对多 个基本表的更新

```
CREATE TRIGGER <触发器名>  
INSTEAD OF [INSERT | DELETE | UPDATE | UPDATE 
OF 列名称[,列名称,...]] ON 视图名称
FOR EACH ROW
 <触发动作体>
```

![image-20250615160808338](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250615160808338.png)

◼ （1）执行该表上的BEFORE触发器； ◼ （2）执行激活触发器的SQL语句； ◼ （3）执行该表上的AFTER触发器。

### 删除触发器

```
DROP TRIGGER <触发器名> ON <表名>;
 DROP TRIGGER Insert_Sal ON Teacher;
```

# 关系数据理论

![image-20250616083909153](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616083909153.png)

![image-20250616084126795](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616084126795.png)

依赖是什么意思？意思是前面那个值确定了的话，后面那个值是唯一确定的，也就是一种推导关系，**相互依赖的关系只需要写一边**

![image-20250616084416081](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616084416081.png)

### 1. 数据冗余太大 (High Data Redundancy)

数据冗余是指相同的数据在数据库中被多次重复存储。

- **学生信息冗余**: 一个学生每多选一门课，他/她的个人信息（`Sname` 姓名, `Age` 年龄, `Dept` 系别）就会被重复存储一次。例如，学生`S1`（李勇）选了三门课，他的姓名、年龄、系别信息就被存储了三次。
- **系别信息冗余**: 一个系别的主任（`Mn`）信息会随着该系的每一个学生、每一门课程的记录而重复。例如，`计算机`系的系主任是`王平`，这个信息在表中重复出现了五次。

------

### 2. 更新异常 (Update Anomalies)

更新异常指当修改某一个数据时，需要修改多处记录，如果漏改了任何一处，就会导致数据不一致。

- **例子**: 假设`计算机`系换了新的系主任，不再是`王平`。为了更新这个信息，我们必须修改所有`Dept`为`计算机`的记录（在本例中是前五条记录）。如果只修改了其中几条，就会出现`计算机`系有两个不同系主任的矛盾情况，破坏了数据的**一致性**。

------

### 3. 插入异常 (Insertion Anomalies)

插入异常指当想要插入某些信息时，却因为缺少其他必要信息而无法插入。

- **例子1 (无法插入新生信息)**: 假设学校新招了一名学生，但他还没有选任何课程。由于主键是`(Sno, Cno)`，而`Cno`（课程号）此时为空，主键的一部分不能为空，所以我们**无法将这位新生的信息（学号、姓名、系别）单独插入**到这个表中。
- **例子2 (无法插入新系别信息)**: 假设学校要新成立一个`物理系`，并任命了系主任。但由于还没有学生属于这个系，`Sno`和`Cno`都为空，我们同样**无法将这个新系别及其主任的信息插入**表中。

------

### 4. 删除异常 (Deletion Anomalies)

删除异常指当删除某些信息时，导致了另一些我们仍需要的信息也一起被意外删除。

- **例子**: 假设学生`S3`（王敏）只选了两门课(`C2`, `C3`)。如果她把这两门课都退掉了，我们就要删除这两条记录。删除这两条记录的同时，关于`S3`这位学生的所有信息（学号、姓名、年龄、所在系别）也会随之**完全消失**。但实际上，这位学生可能只是退了课，学籍仍然在学校。这就造成了有用信息的丢失。

## 函数依赖

对  t ,  s   r ，若 t[X] = s[X] ，则 t[Y] = s[Y] 成立， 那么称“X函数决定Y”，或“Y函数依赖于X”， 记作X→Y

如果X →Y，且YX，则称其为平凡的函数依赖如（Sno，Sname）→ Sname是平凡的函数依赖

 当关系中的元组增加、删除或更新后都不能破坏这种函数 依赖，函数依赖关系的存在与时间无关，而只与数据之间的语义 规定有关。

### 完全函数依赖：

![image-20250616085914842](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616085914842.png)

只有当决定因素是组合属性时，讨论部分函数 依赖才有意义 当决定因素是单属性时，只能是完全函数依赖

![image-20250616090049787](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616090049787.png)

**▪ 全码 整个属性组是码**

### 第一范式 (1NF - First Normal Form) 原子性

**定义**：确保数据表中的所有字段（列）都是**原子性**的，即不可再分。

这是最基本的要求，现代关系数据库系统都会强制遵守。

- **不符合1NF的例子**：一个“地址”列里同时包含了“国家、省份、城市、街道”。
- **符合1NF的做法**：将“地址”拆分为“国家”、“省份”、“城市”、“街道”四个独立的列。

------

### 第二范式 (2NF - Second Normal Form) 消除部分依赖

**定义**：在满足1NF的基础上，表中所有非主键列都必须**完全依赖**于整个主键，而不能只依赖于主键的一部分。

这个范式主要针对的是**复合主键**（由多个列组成的主键）。如果表只有一个单列主键，那么它只要满足1NF，就自动满足2NF。

- 例子

  ：一个订单详情表，主键是 

  ```
  (订单ID, 商品ID)
  ```

  - **表结构**: `(订单ID, 商品ID, 商品名称, 订单日期, 数量)`
  - **问题**: `商品名称` 只依赖于 `商品ID`，`订单日期` 只依赖于 `订单ID`。它们都只依赖于主键的一部分，这就叫**部分依赖**。
  - **后果**: 如果一个商品被100个订单购买，它的名称就会重复存储100次，造成冗余。
  - 2NF改造
    1. **订单表**: `(订单ID, 订单日期)`
    2. **商品表**: `(商品ID, 商品名称)`
    3. **订单详情表**: `(订单ID, 商品ID, 数量)`

------

### 第三范式 (3NF - Third Normal Form) 消除传递依赖

**定义**：在满足2NF的基础上，表中所有非主键列都不能依赖于其他非主键列。简单说，非主键列之间不能有依赖关系，它们必须且只依赖于主键。

- 例子

  ：一个学生信息表，主键是 

  ```
  学号
  ```

  - **表结构**: `(学号, 姓名, 所在院系, 院系主任)`
  - **问题**: `院系主任` 依赖于 `所在院系`，而 `所在院系` 依赖于 `学号`。这就形成了 `学号 → 所在院系 → 院系主任` 的**传递依赖**。
  - **后果**: 如果一个院系换了主任，所有属于该院系的学生记录都需要更新，容易出错。
  - 3NF改造
    1. **学生表**: `(学号, 姓名, 院系ID)`
    2. **院系表**: `(院系ID, 院系名称, 院系主任)`

------

### BCNF (Boyce-Codd Normal Form) 更严格的3NF

**定义**：在满足3NF的基础上，**任何函数依赖关系 `X → Y` 中，`X` 必须是该表的**超键（Superkey）。超键是能唯一标识表中记录的属性或属性组合，主键是一种特殊的超键。**这里讲的是**

BCNF可以被看作是3NF的修正版或增强版，它堵上了3NF中一些比较罕见的漏洞。大多数符合3NF的设计也符合BCNF。

- **区别于3NF的场景**：一个表里有多个候选键，且**这些候选键互相重叠**。

- 例子

  一个选课表，记录学生给老师授课的评价。

  - **表结构**: `(学生, 课程, 老师)`

  - 背景

    - 一个老师只教一门课。(`老师 → 课程`)
    - 一个学生选一门特定的课，只会被一个老师教。(`(学生, 课程) → 老师`)

  - **候选键**: `(学生, 课程)` 和 `(学生, 老师)` 都可以作为主键。

  - **问题**: `课程` 依赖于 `老师`，但 `老师` 并不是超键（不能唯一确定一条记录）。这违反了BCNF。

  - BCNF改造

    1. **师生关系表**: `(学生, 老师)`

    2. **教师授课表**: `(老师, 课程)`

       ![image-20250616093221762](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616093221762.png)

------

### 第四范式 (4NF - Fourth Normal Form) 消除多值依赖

**定义**：在满足BCNF的基础上，消除表中的**多值依赖**。

多值依赖是指，**表中某列的多个值不依赖于另一列**，而是依赖于表中的其他某些列。这个范式处理的是一个实体拥有多个独立的、多值的属性时的情况。

- 例子

  ：一个课程信息表。

  - **表结构**: `(课程, 授课老师, 参考书)`
  - **背景**: 一门课程可以由多个老师教，也可以有多本参考书。老师和参考书之间没有直接关系。
  - **问题**: 为了记录所有组合，会产生大量冗余。比如`物理课`由`张老师`和`李老师`教，参考书是`《大学物理》`和`《物理习题集》`。那么需要存储4条记录来表示所有组合，但老师和书是相互独立的。
  - 4NF改造
    1. **课程-老师表**: `(课程, 授课老师)`
    2. **课程-参考书表**: `(课程, 参考书)`

------

### 第五范式 (5NF - Fifth Normal Form) 消除连接依赖

**定义**：在满足4NF的基础上，消除**连接依赖**，确保表中的信息无法在不产生冗余数据的情况下被分解为更小的表。

5NF也称为**投影-连接范式 (PJ/NF)**。它处理的是最复杂的数据关系，现实中非常罕见，大多数数据库设计在达到BCNF或4NF后就已经足够健壮。只有当一个表可以被无损地分解成三个或更多的表，并且需要将它们全部连接（Join）起来才能恢复原始信息时，才可能涉及5NF。

**一原子 二全依赖主键 三只依赖主键 （三nf说每一个非主键列要只依赖于主键列 但是我有找重叠的主键列不炸了吗）bcnf说要是主键有重复的话**

![image-20250616094337893](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616094337893.png)

## Armstrong公理系统

 自反律(reflexivity)：若Y 属于 X， 则X →Y

 ▪ 增广律(augmentation)：若X → Y ，则XZ →YZ

 ▪ 传递律(transitivity)：若X → Y，Y → Z，则X →Z

▪合并律(union rule) •若X →Y，X →Z，则X →YZ

▪分解律(decomposition rule) •若X →Y，及 Z Y 则 X →Z

伪传递律(pseudo-transitivity rule) •若X →Y，WY →Z，则WX →Z

### 闭包

 属性集的闭包 ◼设F为属性集U上的一组函数依赖，X U， X + F = {A | X→A能由F根据Armstrong公理导出} X + 称      为属性集X关于函数依赖集F的闭包

![image-20250616100542359](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616100542359.png)

**计算出F的所有蕴含F+实际上不可行 但是我们可以结算X+**

![image-20250616100911895](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616100911895.png)

![image-20250616101735111](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616101735111.png)

![image-20250616103925696](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616103925696.png)

### 函数依赖的等价和覆盖

![image-20250616110022351](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616110022351.png)

![image-20250616110100826](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616110100826.png)

也就是说，做好第一步之后，看右边，又看左边

### 模式分解

![image-20250616110744407](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616110744407.png)

也就是说，几个属性有蕴含关系，属于函数依赖，但是几个袁术组合为一个关系模式，对应里面的函数依赖

![image-20250616141633530](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616141633530.png)

投影后要自然链接（自然链接会自动匹配相同的地方然后链接）

![image-20250616142342106](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616142342106.png)

![image-20250616145539245](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616145539245.png)

![image-20250616145658755](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616145658755.png)

![image-20250616151338022](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616151338022.png)

![image-20250616151349508](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616151349508.png)

#### **第一步：尝试应用 `AB -> C`**

- **触发条件**: 寻找在 `A` 列和 `B` 列上值都相同的行。

- 检查

  :

  - 第一行: (A=`a1`, B=`a2`)
  - 第二行: (A=`b21`, B=`b22`)
  - 第三行: (A=`b31`, B=`b32`)

- **结果**: 没有两行的 `A` 和 `B` 值是相同的。因此，这条规则**无法触发任何修改**。右上角的表格和左上角的初始表完全一样，它只是表示“我们已经考虑过 `AB -> C` 这条规则了”。

#### **第二步：应用规则 `C -> D` (图中的关键变化)**

- **触发条件**: 寻找在 `C` 列上值相同的行。

- 检查

  :

  - 第一行 ("ABC"): `C` 列的值是 **`a3`**。
  - 第二行 ("CD"): `C` 列的值是 **`a3`**。
  - 第三行 ("DE"): `C` 列的值是 `b33`。

- **发现**: 第一行和第二行的 `C` 值相同！

- 执行操作

  : 算法要求这两行的 

  ```
  D
  ```

   值也必须相同。

  - 第一行的 `D` 值是 `b14` (b 类符号)。
  - 第二行的 `D` 值是 `a4` (**a 类符号**)。

- 结果

  : 根据“常量优先”原则 (规则 a)，

  ```
  a
  ```

   类符号会覆盖 

  ```
  b
  ```

   类符号。因此，算法将第一行的 

  ```
  b14
  ```

   修改为 

  ```
  a4
  ```

  。

  - 这正是图中**右下角表格**所展示的变化，并用**红色圆圈**高亮了出来。

#### **第三步：应用规则 `D -> E` (图中未画出，但算法会继续)**

在执行完第二步后，我们的表格变成了右下角的样子。算法会继续尝试下一条规则。

- **触发条件**: 寻找在 `D` 列上值相同的行。

- 检查

  :

  - 第一行 ("ABC"): `D` 列的值是 **`a4`** (刚刚被修改的)。
  - 第二行 ("CD"): `D` 列的值是 **`a4`**。
  - 第三行 ("DE"): `D` 列的值是 **`a4`**。

- **发现**: 现在三行的 `D` 值都相同了！

- 执行操作

  : 算法要求这三行的 

  ```
  E
  ```

   值也必须相同。

  - 第一行的 `E` 值是 `b15` (b 类)。
  - 第二行的 `E` 值是 `b25` (b 类)。
  - 第三行的 `E` 值是 `a5` (**a 类**)。

- **结果**: 同样根据“常量优先”原则，`a5` 会覆盖 `b15` 和 `b25`。第一行和第二行的 `E` 值都被修改为 `a5`。

### 简单方法

关系模式R(U)的分解={R1，R2}，则是一个 无损连接分解的充要条件是 R1∩R2→R1−R2 （或R1∩R2→R2−R1）成立

![image-20250616153647081](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616153647081.png)

### 保持函数依赖的模式分解

![image-20250616154050786](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250616154050786.png)

意思是说有一个全集的子集z，然后呢f在z上面的投影就是x和y在z里面而且构成依赖关系，全集u的依赖关系等于每一个自己的子集的并再求依赖关系

 三个重要事实：

 ▪ （1）若要求分解保持函数依赖，那么模式分解总可 以达到3NF，但不一定能达到BCNF；

 ▪ （2）若要求分解既保持函数依赖，又具有无损连接 性，可以达到3NF，但不一定能达到BCNF；

 ▪ （3）若要求分解具有无损连接性，则一定可以达到 4NF

# 数据库设计

 掌握根据需求画E-R图。 

 掌握根据E-R图转为关系模式并明确主码及外码。

![image-20250617090101513](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617090101513.png)

![image-20250617090311746](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617090311746.png)

◼ 派生属性可以从其他相关的属性或实体派生出来的 属性值 ◼ 数据库中，一般只存基属性值，而派生属性只存其 定义或依赖关系，用到时再从基属性中计算出来 ◼ 勿轻易引入派生属性

![image-20250617090420544](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617090420544.png)

![image-20250617090516166](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617090516166.png)

当同一个实体集**不止一次参与一个联系集**时，为区别 各实体的参与联系的方式，需要显式指明其角色

![image-20250617091028788](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617091028788.png)

### **① 规则一：1对1 (1:1) 关系**

- **规则原文**：`1:1时: 取任何一方的KEY作为联系的KEY`

- **含义**：当两个实体是**一对一**关系时，你可以将任意一个实体的主键（Primary Key）放到另一个实体的数据表中作为外键（Foreign Key）。

- 图中示例

  ：

  - **实体**：“班级” (Class) 和 “班长” (Class Monitor)。
  - **关系**：“管理” (Manages)。一个班级只有一个班长，一个班长只管理一个班级，这是典型的1:1关系。
  - **实现方法**：你可以在“班级”表中增加一个`班长学号`字段（引用“班长”表的主键`sno`），或者在“班长”表中增加一个`班级号`字段（引用“班级”表的主键`clno`）。两者选其一即可。

------

### **② 规则二：1对多 (1:M) 关系**

- **规则原文**：`1:m时: 取“m”方的KEY作为联系实体的KEY` (这里原文的表述容易引起误解，标准做法是反过来的)
- **标准含义**：在**一对多**关系中，应将“1”那一端实体的主键，添加到“多 (m)”那一端实体的数据表中作为外键。
- 图中示例
  - **实体**：“班级” (Class) 和 “学生” (Student)。
  - **关系**：“拥有” (Has/Contains)。一个班级拥有多名学生，但一名学生只属于一个班级，这是典型的1:M关系。
  - **实现方法**：你应该在“学生”表（多方）中增加一个`班级号`字段 (`clno`)，用它来关联“班级”表（一方）。这样，每个学生记录都能明确指向其所属的班级。

------

### **③ 规则三：多对多 (M:N) 关系**

- **规则原文**：`m:n时: 取“双方”的KEY作为联系实体的KEY`

- **含义**：当两个实体是**多对多**关系时，你**必须**创建一个全新的、独立的“关系表”（也叫连接表或中间表）来表示这个关系。

- 图中示例

  ：

  - **实体**：“学生” (Student) 和 “课程” (Course)。

  - **关系**：“选修” (Takes/Selects)。一个学生可以选修多门课程，同时一门课程也可以被多个学生选修，这是典型的M:N关系。

  - 实现方法

    ：

    1. 创建一个名为“选修”的新表。
    2. 这个新表至少包含两个字段：`sno`（来自学生表的主键）和 `cno`（来自课程表的主键）。
    3. 这两个字段共同组成“选修”表的**复合主键** (Composite Primary Key)。
    4. 如果关系本身有属性，比如图中的`p`（很可能代表`成绩` - points/grade），也应作为“选修”表的一个字段。

![image-20250617091802397](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617091802397.png)

![image-20250617092151739](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617092151739.png)

![image-20250617092220975](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617092220975.png)

![image-20250617092619350](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617092619350.png)

![image-20250617093019610](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617093019610.png)

![image-20250617093101314](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617093101314.png)

#### 注意箭头

![image-20250617093358209](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617093358209.png)

![image-20250617093854569](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617093854569.png)

### 子类的三种方法：

指向继承，面向对象 继承属性你，自己再加自己的属性

![image-20250617105631504](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617105631504.png)

ER 主键链接

![image-20250617105649635](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617105649635.png)

使用空值 父类用子类 没有的用null

![image-20250617105710304](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617105710304.png)

## 聚集

![image-20250617105743894](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617105743894.png)

![image-20250617105945964](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617105945964.png)

![image-20250617105955387](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617105955387.png)

![image-20250617110952943](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617110952943.png)

**关系变成er图 要结合前面的内容**

![image-20250617111210176](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617111210176.png)

#  关系查询处理及查询优化

## 选择的实现

全表扫描和索引扫描（属性上有索引）

## 链接操作的实现

```
］Select * from student ，SC 
WHERE Student.Sno=SC.Sno；
```

### 嵌套循环方法（nested loop）

对于外层的每一个元组检查内部的每一个元组（两层for） 满足条件链接属性输出

### 排序-合并方法（sort-merge join 或merge join）

按照条件（sno）先排序，对于外面的表提取条件属性进行比较，直到第一个属性不相同

### 索引链接方法

在外层的表上建立**条件属性的索引**，对于内层的表的每一个元组通过条件查找外层sc的索引

### 哈希链接

把连接属性作为hash码，用同一个hash函数把R和S中的元 组散列到同一个hash文件中

 划分阶段(partitioning phase)： ▪ 对包含较少元组的表(比如R)进行一遍处理 ▪ 把它的元组按hash函数分散到hash表的桶中

 试探阶段(probing phase)：也称为连接阶段(join phase)  ▪ 对另一个表(S)进行一遍处理 ▪ 把S的元组散列到适当的hash桶中 ▪ 把元组与桶中所有来自R并与之相匹配的元组连接起来

## 代数优化

![image-20250617140951514](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617140951514.png)

![image-20250617141038250](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617141038250.png)

第二个 和谁无关就独立谁 有关就上套

![image-20250617141240062](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617141240062.png)

## 启发式搜索

![image-20250617141828870](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617141828870.png)

![image-20250617142548383](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617142548383.png)

**拆分选择条件**

![image-20250617142611714](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617142611714.png)

将选择动作尽早进行 挪到下面不可以再挪的地方：

![image-20250617142918718](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617142918718.png)

把选择运算与其前面的 笛卡尔积运算结合起来， 用内连接代替笛卡尔积。

![image-20250617142954793](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617142954793.png)

消除不必要的属性 减少中间规模，也就是在自然链接之前先选一次需要的属性

![image-20250617143136827](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617143136827.png)

![image-20250617143329524](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617143329524.png)

# 数据库恢复

## 掌握事务的四个特性

acid 记住英文

隔离性(Isolation) 多个事务**并发执行时**，系统必须**保证事务的执行不被 其他事务干扰**。

每个事务都感觉不到系统中有其他事 务在并发地执行。 

对任何一对事务T1，T2，在T1看来，T2要么在T1开始 之前已经结束，要么在T1完成之后再开始执行

![image-20250617144617524](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617144617524.png)

- 我们假设变量 A 的初始值是 `16`。
- **(1) 事务T1** 读取 A，得到 A = 16。
- **(2) 事务T2** 在T1提交之前，也读取 A，同样得到 A = 16。
- **(3) 事务T1** 计算 A = A - 1 (16 - 1 = 15)，并将结果 `15` 写回数据库。
- **(4) 事务T2** 基于它之前读到的旧值 `16` 计算 A = A - 3 (16 - 3 = 13)，并将结果 `13` 写回数据库。

**为什么这是错误的（为什么没有隔离性）？** 这个过程导致了经典的“**丢失更新**”（Lost Update）问题。事务T1的更新操作（将A更新为15）实际上丢失了，因为它立刻被事务T2的更新（将A更新为13）所覆盖。

**正确的结果应该是什么？** 如果数据库保证了良好的隔离性，这两个事务应该像串行执行一样，结果是唯一的。

- **情况一 (T1先执行完):** A从16变成15，然后T2读取15，计算后A最终变成`12`。
- **情况二 (T2先执行完):** A从16变成13，然后T1读取13，计算后A最终变成`12`。

无论如何，正确的结果都应该是 `12`。但图中的错误执行流程导致结果变成了 `13`。

![image-20250617150531248](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617150531248.png)

## 登记日志文件遵循的两条原则。

### 日志

用来记录事务对数据库的更新操作的文件

以记录为单位 以数据块为单位

![image-20250617150216886](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617150216886.png)

**1.日志缓冲区 ◼ 在内存中开辟的临时保存日志记录的区域 ◼ 根据需要一次将一个或多个缓冲块写入磁盘，从而 减少写磁盘的次数。 ◼ 写到磁盘中的日志记录顺序必须与写入日志缓冲区 的顺序完全一致。**

**2.记录时机 ◼ 必须先写日志文件，后写数据库**

## 恢复策略及步骤：事务故障；系统故障；介质故 障

使用日志文件进行恢复的思想（ARIES）

* *1.从后往前扫描日志，构造undo-list和redo-list： ◼ 对每一个形如的记录，将Ti 加入redo-list。  ◼ 对每一个形如的记录，如果Ti不属于 redo-list，则将Ti加入undo-list。*  

* 由后至前重新扫描日志，对undo-list中的每个事务Ti的每 一个日志记录执行undo操作。 

* 由前至后重新扫描日志，并且对redo-list中每个事务Ti的 每一个日志记录执行redo操作。

  **因为undo是撤销事物重做 redo是重新提交 undo必然是逆向世界逐渐恢复**

  #### 事务故障的恢复（UNDO）

   ①反向扫描日志文件(即从最后向前扫描日志文件)，查找该事 务的更新操作。 ②对该事务的更新操作执行逆操作，即将日志记录中“更新前 的值”写入数据库。 ③继续反向扫描日志文件，查找该事务的其他更新操作，并做 同样处理。 ④如此处理下去，直至读到此事务的开始标记，事务故障恢复 就完成了

  #### 系统故障的恢复（UNDO＋REDO）

  ①正向扫描日志文件，找出在故障发生前已经提交事务，将其 事务标识记入重做(REDO)队列。同时还要找出故障发生时 尚未完成的事务，将其事务标识记入撤销(UNDO)队列。 ②反向扫描日志文件，对撤销队列中的各个事务进行撤销 (UNDO)处理。 ③正向扫描日志文件，对重做队列中的各个事务进行重做 (REDO)处理。

   #### 介质故障（磁盘损坏 要大规模重建）的恢复（REDO）

  ①装入最新的后备数据库副本，使数据库恢复到最近一次转储 时的一致性状态。 对于动态转储的数据库副本，还须同时装入转储开始时刻 的日志文件副本，利用与恢复系统故障相同的方法(即 REDO+UNDO)，才能将数据库恢复到一致性状态。 ②装入有关的日志文件副本（转储结束时刻的日志文件副本）， 重做已完成的事务。

  数据转储tips：转储时机 ◼ 静态转储：无事务执行 ◼ 动态转储：边执行事务，边转储 转储内容 ◼ 海量转储：转储全部数据库 ◼ 增量转储：只转储更新过的数

## 握带检查点的恢复步骤

周期性地对日志做检查点，以避免故障恢复时检查 整个日志。

在日志文件中增加一类新的记录——检查点 (Checkpoint)记录，增加一个重新开始文件，并让 恢复子系统在登录日志文件期间动态地维护日志。

Checkpoint记录内容 ◼ Checkpoint时刻所有正在执行的事务清单 ◼ 这些事务最近一个日志记录的地址

![image-20250617152111241](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617152111241.png)

### 动态维护日志文件的方法

也就是，我们的日志先放在一个缓冲区，到了那个checktime会把日志缓冲写进主存的日志文件，写进去之后，加入checkpoint，同时把数据页commit到底数据库里面，然后把checkpoin的位置写到日志文件里面

![image-20250617154226172](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617154226172.png)

t1在checkpoint之前完成，不用做任何事情，t2t4在故障前完成，但是checkpoint没有查到要redo t3和t4在故障前没有完成要undo

用list实现

# 并发控制

![image-20250617163532878](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617163532878.png)

![image-20250617170317407](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617170317407.png)

![image-20250617170324917](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617170324917.png)

![image-20250617170507437](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617170507437.png)

## 可串行性

多个事务以任意串行 方式执行都能保证DB 的正确性

若某并发调度S与一个**串行调度的执行有相同的效果， 则称调度S是可串行化的（可串行化的调度）**

一个调度Sc在保证**冲突操作（写读和写写）**的次序不变的情况下，通 过交换两个事务不冲突操作的次序得到另一个调度 Sc’，如果Sc’是串行的，称调度Sc为冲突可串行化 的调度

一个调度是冲突可串行化，一定是可串行化的调度（后能推前）冲突可串行化调度是可串行化调度的充分条件，不是必要 条件。还有不满足冲突可串行化条件的可串行化调度。

只有谢谢和读写操作是冲突操作 其他都不是冲突操作

不同事务的冲突操作和同一事务的两个操作两者内部不能交换 (Swap)

![image-20250617190025534](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617190025534.png)

## 可回复的并发调度

对于每一对事务Ti 和Tj ，如果Tj 读取了由Ti 所写的数据项，则Ti 先于Tj 提交。

#### 无级联调度

无级联调度应满足 ◼ 对于每对事务Ti 和Tj ，如果Tj 读取了由Ti 所写的数据项， 则Ti 必须在Tj 这一读取前提交

若某调度是无级联调度，则该调度一定是可恢复调度。

（级联调度：：因一个 事务故障导致一系 列事务回滚的现象）

## 封锁

事务T可以向系统发出请求，对某个数据对象加锁 (Lock)，于是事务T对这个数据对象就有一定的控制， 直到T释放它的锁为止(Unlock事务T可以向系统发出请求，对某个数据对象加锁 (Lock)，于是事务T对这个数据对象就有一定的控制， 直到T释放它的锁为止(Unlock)

排他锁：其他事物不能上锁，不能读取和修改（x）

共享锁：其他数据可以给加了s锁的加s锁，但是其他事物不能修改只能读

**封锁对象的大小称为封锁的粒度**

## 一级封锁

事务Ti在修改 数据Di之前须 先对Di加X锁， 直到事务Ti结 束才释放。

![image-20250617191339531](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617191339531.png)

防止丢失修改 保证Ti可恢复

## 二级

在一级封锁协 议加上事务Ti在 读取Di之前须 先对Di加S锁**， 读完**后即可释 放该S锁。

![image-20250617191348505](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617191348505.png)

防止丢失修改 防止读脏

## 三级

在一级封锁协议 上加上Ti读Di前 须先对Di加S锁，** 直至Ti结束后**才 释放该S锁

防止丢失修改 防止读脏 防止不可重复读

## 活锁

某事务T等待对数据项加排它锁，而另一个事务 序列中的每一个事务都对该数据项加共享锁，则 事务T永远得不到进展

![image-20250617192209787](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617192209787.png)

## 死锁

![image-20250617192228306](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617192228306.png)

### 等待图

![image-20250617192354071](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617192354071.png)

![image-20250617192456638](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617192456638.png)

### 解除死锁

选择一个处理死锁代价最小的事务，将其撤销 释放此事务持有的所有的锁，使其它事务能继续运 行下去

## 两段锁

要求每个事务分两个阶段提出加锁和解锁申请

**扩展阶段：事务可以获得锁，但不能释放锁**

**收缩阶段：事务可以释放锁，但不能获得新锁**

**在对任何数据读、写之前，须先获得该数据锁 在释放一个封锁之后，该事务不能再申请任何其它 锁**

**若所有并行事务都遵守2PL协议，则对这些事务的所有 并行调度策略都是可串行化的。**

◼ 两段锁协议与防止死锁的一次封锁法 一次封锁法要求每个事务必须一次将所有要使用的数 据全部加锁，否则就不能继续执行，因此一次封锁法 遵守两段锁协议 两段锁协议并不要求事务必须一次将所有要使用的数 据全部加锁，因此遵守两段锁协议的事务可能发生死 锁

假设有两个事务，T1和T2，以及两个数据项（比如两个银行账户），A和B。

- 事务T1

  : 想要从账户A转100元到账户B。

  - 它需要先锁住A，再锁住B。

- 事务T2

  : 想要从账户B转200元到账户A。

  - 它需要先锁住B，再锁住A。

现在，我们来看一种可能的并发执行顺序，它完全遵守两段锁协议：

1. **时刻1: T1启动，进入扩展阶段，成功锁住账户A。**
   - T1持有A的锁。
2. **时刻2: T2启动，进入扩展阶段，成功锁住账户B。**
   - T2持有B的锁。这在当时是允许的，因为T1还没有去锁B。
3. **时刻3: T1尝试去锁住账户B，以便进行转账。**
   - **失败并等待！** 因为账户B已经被T2锁住了。T1现在进入等待状态，等待T2释放B的锁。
4. **时刻4: T2尝试去锁住账户A，以便进行转账。**
   - **失败并等待！** 因为账户A已经被T1锁住了。T2现在也进入等待状态，等待T1释放A的锁。

**僵局形成：**

- T1持有A，等待T2释放B。
- T2持有B，等待T1释放A。

双方都持有对方需要的资源，同时又在等待对方释放资源，谁也无法前进。这就是一个典型的**死锁（Deadlock）**。

**如果要求“一次性加锁”，会怎样**

现在我们反过来思考，如果协议要求**“事务必须在开始时就一次性锁住所有需要的数据”**（这被称为静态或保守的2PL），那么上面的情况就不会发生。

- T1会尝试同时锁住A和B。
- T2也会尝试同时锁住A和B。

在这种“要么全部拿到，要么一个都别拿”的策略下，只有一个事务能成功。比如T1先成功锁住了A和B，那么T2在尝试加锁时会立刻失败并等待，它不可能只锁住一个B。这样就不会出现“互相持有对方所需资源”的局面，也就**避免了死锁**。

## 封锁粒度

 封锁对象的大小称为封锁粒度(Granularity)

封锁的粒度越大，数据库所能够封锁的数据单元就越 少，并发度就越小，系统开销也越小； 封锁的粒度越小，并发度较高，但系统开销也就越大。

![image-20250617194240953](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617194240953.png)

![image-20250617194252912](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617194252912.png)

多粒度封锁协议 ◼ 允许多粒度树中的每个结点被独立地加锁 对一个结点加锁意味着这个结点的所有后裔结点也被加 以同样类型的锁 ◼ 在多粒度封锁中一个数据对象可能以两种方式封锁 显式封锁: 直接加到数据对象上的封锁 隐式封锁: 该数据对象没有独立加锁，是由于其上级结 点加锁而使该数据对象加上了锁 显式封锁和隐式封锁的效果是一样的

 该数据对象 ➢ 有无显式封锁与之冲突  所有上级结点 ➢ 检查本事务的显式封锁是否与该数据对象上的隐式 封锁冲突：(由上级结点已加的封锁造成的） 所有下级结点 ➢ 看上面的显式封锁是否与本事务的隐式封锁（将加 到下级结点的封锁）冲突

## 意向锁

 如果对一个结点加意向锁，则说明该结点的下层结点正在被 加锁  对任一结点加基本锁，必须先对它的上层结点加意向锁 

◼ 例如，对任一元组加锁时，必须先对它所在的数据库和关系加 意向锁

◼ 意向共享锁(Intent Share Lock，简称IS锁)  如果对一个数据对象加IS锁，表示它的后裔结点拟（意向） 加S锁。

◼ 意向排它锁(Intent Exclusive Lock，简称IX锁)  如果对一个数据对象加IX锁，表示它的后裔结点拟（意向） 加X锁。

 ◼ 共享意向排它锁(Share Intent Exclusive Lock，简称 SIX锁)  如果对一个数据对象加SIX锁，表示对它加S锁，再加IX锁， 即SIX = S + IX。

- **S (共享锁):** “我要**读**整张表，在我读完前，谁也**不许修改**表里的任何东西。”
- **X (排他锁):** “我要**修改**这张表（或者表里的某几行），在我搞定前，**谁都别碰**这张表，读也不行。”
- **IS (意向共享锁):** “我**没想**对整张表做什么，但我**准备**去锁住里面的**几行来读**。” (这是一个“路过”的声明)
- **IX (意向排他锁):** “我**没想**对整张表做什么，但我**准备**去锁住里面的**几行来改**。” (这也是一个“路过”的声明)
- **SIX (共享意向排他锁):** “我要**读整张表**，并且，我还**准备修改**其中的几行。” (S 和 IX 意图的结合)

现在，我们来逐一分析矩阵中的冲突关系，你就会发现一切都顺理成章了。

------

### ## 为什么是这个结果？逐条分析

我们以 T1 **已持有锁**，T2 来**请求锁**为例：

#### **1. 如果 T1 持有 X 锁 (排他锁)**

- **T1的意图**：“整张表都是我的，谁也别碰！”
- **结论**：T2 无论申请任何锁（S, X, IS, IX, SIX），都会和 T1 “独占全表”的意图冲突。所以矩阵中 X 锁这一行，除了它自己，全是 `N` (No)。

------

#### **2. 如果 T1 持有 S 锁 (共享锁)**

- **T1的意图**：“我要读全表，请保持安静，别修改任何东西。”

- T2来申请

  ：

  - `S` 或 `IS` 锁：T2 也想读（全表或几行），意图都是“读”，不冲突。**所以是 `Y` (Yes)。**
  - `X`, `IX`, 或 `SIX` 锁：T2 想要修改（全表或几行），这和 T1 “别修改任何东西”的意图严重冲突。**所以是 `N` (No)。**

------

#### **3. 如果 T1 持有 IX 锁 (意向排他锁)**

- **T1的意图**：“我准备进去改几行。”

- T2来申请

  ：

  - `IS` 锁：T2 也想进去读几行。T1改的和T2读的**大概率不是同一行**，即使是，冲突也会在行锁的层面解决。在表这个层面，两个“意向”不冲突。**所以是 `Y` (Yes)。**
  - `IX` 锁：T2 也想进去改几行。和上面同理，只要不改同一行就没事。两个“想进去改东西”的意向在表层面不冲突。**所以是 `Y` (Yes)。**
  - `S` 或 `SIX` 锁：T2 想锁定全表来读。这和 T1 “我准备进去改几行”的意图**严重冲突**。因为 T1 的修改会破坏 T2 “读全表”的一致性。**所以是 `N` (No)。**

------

#### **4. 如果 T1 持有 IS 锁 (意向共享锁)**

- **T1的意图**：“我准备进去读几行。” 这是最“佛系”的意向。
- **结论**：除了 T2 想用 `X` 锁独占全表之外，其他的意图（S, IS, IX, SIX）都和 T1 这个“只想读几行”的意图不冲突。所以 IS 这一行，除了对 X 是 `N`，其他都是 `Y`。

------

#### **5. 如果 T1 持有 SIX 锁 (共享意向排他锁)**

- **T1的意图**：“我要读全表，并且我还准备改几行。” 这是一个很强的锁。

- T2来申请

  ：

  - `IS` 锁：T2 只想进去读几行。T1 允许别人一起读全表，所以也允许 T2 读几行。**所以是 `Y` (Yes)。**
  - 任何其他锁 (`S`, `X`, `IX`, `SIX`): 都会和 T1 “我要读全表” 或 “我准备改几行” 的意图产生冲突。比如 T2 申请 `IX`（想进去改），就和 T1 的 `S`（读全表）部分冲突。T2 申请 `S`（想读全表），就和 T1 的 `IX`（想改几行）部分冲突。**所以都是 `N` (No)。**

![image-20250617195937125](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250617195937125.png)

# 存储

**数据库的数据则是以文件为单位组织，文件在物理上又划分为文件块，一个关系会占用一到多个物理块，而一个块存放关系的多个记录，即“文件-块-记录”的物理组织形式。**

![image-20250618084240115](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250618084240115.png)

**同时 主存也在这个数据库在磁盘中存储的文件页索引的存储方式**

### 数据库结构

文件素质为页 有数据文件和索引文件

### 堆文件

目录页存储页的位置和每个页面的空闲空间信息

### 页设计

元组、元数据、索引、日志记录等。数据一般不混合存放。每个页有位移页面id

每个页有 **页头：元数据和脏位** ；**数据区域**

## 基于槽页的面向元组型的页设计

![image-20250618085821609](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250618085821609.png)

在页头之后建立一个槽数组。假设此时插入一个元组，我们就从块尾分配一个区域存放该元组，在槽数组中的第一个位置记录该元组在页内的偏移地址，当插入第二个数组时，重复刚才的动作。这样在槽数组和元组中的区域就是空闲区域。在这种结构中，即使元组不等长，并不影响外部对元组的引用，从而轻松应对定长和变长元组。（鼠标点一下），当删除一个元组时，如删除元组3，我们在元组3所对应的槽标记为空闲。此时在元组4和元组2之间出现碎片，（鼠标点一下）通过移动元组4的位置，保证了空闲区域的连续性，同时改变槽4对元组4的指针保证了元组4对外的地址不变（**把原来指向3的地址的slot指向4 并且移动4）。满足了我们刚开始提出来的需求。为了方便插入元组，我们通常在页头记录已占用的槽位以及上一次使用槽位的开始位置。在槽页这种设计方式下，元组的物理位置，也被视为TupleID，tupleID由文件位置+元组所在页的pageID+在页内的slotID构成。

# B+树

一棵B+树由三部分构成，分别是根节点，内部结点和叶子结点。和所有的树结构一样，只有一个根节点，。叶子结点和前面介绍的表索引一样，它**包含了指向元组记录的指针**，我们发现无论哪个叶子结点，它们到根节点的路径长度都是一样的

![image-20250618090228671](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250618090228671.png)

叶子结点也是一个索引页 叶子结点多两个指针 一个指向有兄弟的页的位置 一个指向左兄弟的页的位置

### 非聚簇索引（ Nonclustered Indexes）

这个的叶子结点指向的是数据的地址，n个文件的第m页，其在页内的偏移地址是k，非聚簇索引和堆文件中元组存放的顺序没有关系，也就是说重构非聚簇索引不改变元组在堆文件中的存放。

![image-20250618100913360](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250618100913360.png)



### 聚簇索引（Clustered Indexes）

要求元组存放的顺序和键的顺序一致，当叶子结点中的值就是键所对应的元组本身，**当然聚簇索引的叶子结点的值也可以存放元组在堆文件中的地址**，但是都要它要求元组存放的顺序和键的顺序一致

![image-20250618101145059](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250618101145059.png)

# 缓冲池

page2不在缓冲池中，则为该页分配空间，并从磁盘读入请求的页面到缓冲池，然后将该页在内存中的地址传给请求者。**还要加载了文件目录，如果文件目录不在内存，必须首先加载它

## LRU-K

LRU-K算法驱逐缓冲池所有帧中K距离最大值的帧。K距离为当前时间戳与倒数第k次访问的时间戳之间的时间差。具有少于k个历史访问的帧的k距离被赋予+inf。当多帧具有+inf的k距离时，替换器将采用普通LRU淘汰具有最早时间戳的帧。

两个list

**第一个list为历史list 按照lru淘汰如果一个值在历史list里面被访问了k次 移动到缓存list**

**缓存list里面，也是进入查询 如果查到了 就把那个值放在首位，其他顺序向后退 当有物质进入缓存list的时候淘汰队尾的**

# 查询

![image-20250618095241811](C:\Users\francis\AppData\Roaming\Typora\typora-user-images\image-20250618095241811.png)

迭代模型（Iterator Model ）物化模型（Materialization Model）向量/批量模型（Vectorized / Batch Model）

每个算子都实现3个函数：Open()初始化；申请空间和设置连接、选择条件的参数Next()获取下一条/一批记录；验证是否满足条件，若满足返回父节点；否则中止Close()清理从根结点开始执行，自上而下调用算子的Next()函数从子结点“拉取”数据

#### 1. 迭代模型（Iterator Model）

- **解释：** 迭代模型，也称为“火山模型”（Volcano Model），是数据库系统中普遍采用的执行模型。它的核心思想是每个操作符都实现一个`next()`（或`pull()`）方法。当上层操作符需要数据时，它会调用下层操作符的`next()`方法，下层操作符再递归地调用其下层的`next()`方法，直到最底层的数据源（如表扫描）。数据以元组（tuple）为单位逐个向上流动。

- **优点：**

  - **内存效率高：** 一次只处理一个元组，不需要将中间结果全部加载到内存中。
  - **易于实现：** 模块化设计，每个操作符只需关注其自身的逻辑。
  - **支持流水线（Pipelining）：** 数据可以在操作符之间直接传递，无需中间结果落地，减少I/O。

- **缺点：**

  - **函数调用开销大：** 每个元组都需要进行多次函数调用，对于大量数据而言，这个开销会显著。
  - **CPU缓存不友好：** 每次处理一个元组，可能导致CPU缓存的频繁失效，降低效率。

- **用图示查询树解释过程：**

  假设我们要执行的查询树是： ΠS.SName, SC.CNO(S⋈S.SNO=SC.SNOσC.Cno=’C1’(SC))

  1. **最上层操作符 ( Π - Projection ):**
     - 投影操作符（Π) 调用其子操作符（ Join - ⋈ ）的 `next()` 方法来获取一个元组。
  2. **中间操作符 ( ⋈ - Join ):**
     - 连接操作符（⋈）在其内部循环中：
       - 它会调用左侧子操作符（ S ）的 `next()` 方法获取 S 表的一个元组。
       - 它会调用右侧子操作符（ σ ）的 `next()` 方法获取 σC.Cno=’C1’(SC) 的一个元组。
       - 当获得两个元组后，它会检查连接条件（`S.SNO=SC.SNO`）。如果满足，则将连接后的元组传递给上层的 Π 操作符。
       - 这个过程会一直重复，直到所有可能的连接都完成。
  3. **右下层操作符 ( σ - Selection ):**
     - 选择操作符（σ) 调用其子操作符（ SC ）的 `next()` 方法来获取 SC 表的一个元组。
     - 获取元组后，它会检查选择条件（`C.Cno='C1'`）。如果满足，则将该元组传递给上层的 Join 操作符；否则，它会继续调用 SC 的 `next()` 方法获取下一个元组。
  4. **最下层操作符 ( S 和 SC - Table Scan ):**
     - 表扫描操作符（S 和 SC）负责从磁盘读取数据，每次调用 `next()` 方法时，它们会返回表中的一个元组。

  整个过程就像一个倒置的火山，数据从底部（表）冒出，经过各个操作符的处理，最终从顶部（投影）输出结果。

#### 2. 物化模型（Materialization Model）

- **解释：** 物化模型与迭代模型相反，它要求每个操作符在完成所有输入处理后，将其所有的结果“物化”（即完全计算并存储）到一个中间数据结构（如临时表、缓冲区）中，然后将这个完整的中间结果传递给上层操作符。上层操作符再从这个物化后的结果中读取数据进行处理。
- **优点：**
  - **简单：** 每个操作符独立完成工作，逻辑更清晰。
  - **可重复使用：** 物化后的中间结果可以被多个上层操作符共享，或者用于调试。
  - **对某些操作友好：** 对于需要多次扫描输入的聚合操作（如排序、哈希连接），物化可以避免重复读取。
- **缺点：**
  - **内存开销大：** 需要为每个中间结果分配大量的内存或磁盘空间，如果中间结果集非常大，可能会导致性能下降甚至系统崩溃。
  - **无法流水线：** 无法实现流水线执行，因为一个操作符必须等到其所有子操作符完成并物化结果后才能开始工作。
  - **I/O 开销大：** 中间结果的读写可能会导致额外的磁盘 I/O。
- **用图示查询树解释过程：**
  1. **右下层操作符 ( σ - Selection ):**
     - 选择操作符（σ) 首先扫描整个 SC 表，并应用条件 `C.Cno='C1'`。
     - 所有满足条件的元组都被物化到一个临时的结果集 `Temp_SC_C1` 中。
     - 一旦 `Temp_SC_C1` 完全生成，这个结果集就被传递给上层的 Join 操作符。
  2. **中间操作符 ( ⋈ - Join ):**
     - 连接操作符（⋈) 接收到 `Temp_SC_C1`。
     - 它同时扫描整个 S 表。
     - 然后，它将 S 表的每个元组与 `Temp_SC_C1` 中的每个元组进行连接条件（`S.SNO=SC.SNO`）的比较。
     - 所有满足连接条件的元组被物化到一个新的临时结果集 `Temp_Join_Result` 中。
     - 一旦 `Temp_Join_Result` 完全生成，这个结果集就被传递给上层的 Π 操作符。
  3. **最上层操作符 ( Π - Projection ):**
     - 投影操作符（Π) 接收到 `Temp_Join_Result`。
     - 它遍历 `Temp_Join_Result` 中的每个元组，并只保留 `S.SName` 和 `SC.CNO` 两个属性。
     - 最终的结果集被生成并返回给用户。

#### 3. 向量/批量模型（Vectorized / Batch Model）

- **解释：** 向量/批量模型是迭代模型的一种优化，旨在减少函数调用开销并提高CPU缓存利用率。它不像迭代模型那样每次处理一个元组，而是每次处理一批（或一个向量）元组。每个操作符的 `next()` 方法不再返回单个元组，而是返回一个包含多个元组的向量（通常是几十到几千个元组）。操作符的逻辑被设计为对整个向量进行操作，利用CPU的SIMD（Single Instruction, Multiple Data）指令集。
- **优点：**
  - **减少函数调用开销：** 每次函数调用处理多个元组，显著降低了调用次数。
  - **提高CPU缓存利用率：** 批量处理数据，使得数据在CPU缓存中停留的时间更长，减少缓存未命中。
  - **更好的CPU指令集利用：** 能够利用SIMD指令进行并行计算，进一步提高性能。
  - **流水线：** 仍然保持了流水线的特性，数据在操作符之间以批量形式流动，无需完全物化。
- **缺点：**
  - **实现复杂度高：** 操作符的实现需要针对批量处理进行优化，比单元组处理更复杂。
  - **不适用于所有操作：** 对于某些操作（如涉及复杂谓词或用户定义函数的场景），批量处理的优势可能不明显。
- **用图示查询树解释过程：**
  1. **最下层操作符 ( S 和 SC - Table Scan ):**
     - 表扫描操作符（S 和 SC）现在每次读取并返回一批（例如1000个）元组，而不是一个。
  2. **右下层操作符 ( σ - Selection ):**
     - 选择操作符（σ) 接收到来自 SC 的一个元组批次。
     - 它不是逐个元组地检查条件，而是对整个批次中的所有元组，批量地应用条件 `C.Cno='C1'`。
     - 结果是一个新的批次，只包含满足条件的元组，并将其传递给上层的 Join 操作符。
  3. **中间操作符 ( ⋈ - Join ):**
     - 连接操作符（⋈) 接收来自 S 的一个批次和来自 σ 的一个批次。
     - 它采用优化的批量连接算法（例如，哈希连接通常可以很好地与批量模型结合），对这两个批次进行连接操作。
     - 结果是一个包含连接后元组的批次，并将其传递给上层的 Π 操作符。
  4. **最上层操作符 ( Π - Projection ):**
     - 投影操作符（Π) 接收来自 Join 的一个批次。
     - 它对整个批次进行投影操作，保留 `S.SName` 和 `SC.CNO` 属性。
     - 最终的结果也是一个批次，并返回给用户。

### 总结

- **迭代模型：** 逐行处理，低内存，高函数调用开销。适合联机事务处理 (OLTP) 或内存受限环境。
- **物化模型：** 逐阶段处理，高内存，无流水线，但对某些操作有优势。在早期的数据库系统或需要中间结果的场景中可见。
- **向量/批量模型：** 迭代模型的优化，批量处理，低函数调用开销，高CPU缓存利用率，流水线。现代分析型数据库系统（OLAP）广泛采用，以实现高性能

第一个是递归，不断往下调用next再逐个返回，第二个是从底部开始用条件去查元组，把符合的加入暂时的数组，再上升进一步寻找；第三个是按批次来，最大的批次是叶子结点，再上升过程中批次量减少，并行进行判断
