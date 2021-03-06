# Terminology and Foundation

数据data：能输入计算机并能被计算机程序处理的所有符号，数据库中存储的基本对象。

数据的语义：数据的具体含义，数据只有被赋予语义后，才能表示确定的意义。

数据库data base，DB：在计算机存储设备上建立起来的用于存储数据的仓库，其中的数据是长期保留，有组织，可共享的数据集合。

数据库管理系统（DataBase Management System，DBMS）：位于用户和操作系统之间的数据管理软件，功能是**数据**定义，组织，存储，管理，操纵以及**数据库**建立，维护，管理。

数据库系统，DataBase System：数据库以及利用数据库技术对计算机中的数据进行管理的计算机系统。包括计算机硬件，数据库，数据库管理系统，数据库应用开发系统在内的计算机软件，以及数据库系统中的人员。

## 数据模型

数据模型：将现实中的事务抽象化，转换为计算机能够处理的数据，使用的工具就是数据模型。

现实世界-概念模型->信息世界-逻辑模型->计算机世界-物理模型->

数据模型的组成要素：数据结构，数据操作和完整性约束。

### 数据的概念模型

实体：客观世界存在并可以区分的事物，如学生。

属性：实体所具有的特性，如学号，姓名。

码：能唯一标识实体的属性集合，如学号，注意“码”是集合，不一定只有一个。

域：属性的取值范围。

实体型：对于一类实体的概括标识，如 学生（学号，姓名，性别）。而将实体性替换为具体的属性值（域中的某一个），就是实体型的一个值。

实体集：同一类型的实体的集合，如全体学生。

联系，实体型内部的联系值的是实体的各个属性之间的联系；实体型之间的联系指的是不同实体集之间的联系。联系有一对一联系，一对多联系，多对多联系，这三种联系关系可以用来两个实体型中的实体的关系，也可描述多个实体型之间的相互关系。

E-R图，Entity-Relationship：用以描述概念模型，可以用来描述实体型，实体型之间的联系。椭圆为属性，棱形表示联系，矩形表示实体型，形状中写对应的名称，形状之间用无向边连接，上面标注数字，表示联系为1:1,1:n,m:n。

### 数据的逻辑模型

逻辑模型由于DBMS的实现，有层次模型，网状模型，关系模型，区别是数据之间联系的表示方式不同。

层次模型类似于树形结构；网状模型允许节点有多个双亲，也允许多个节点没有双亲（多个根节点）；关系模型是目前最重要的一种数据模型。

### 数据的物理模型

物理模型是逻辑模型在计算机中的存储结构。

## 数据库系统的模式与结构

数据库系统采用三级模式和二级映像的系统结构。

模式：用DBMS的描述语言，定义的数据的**逻辑结构**，**数据之间的联系**以及**安全性，完整性要求**。

外模式：通常是模式的一个子集，是一个用户能够看到的局部数据的逻辑结构和特征的描述。应用程序的编写依赖于外模式。外模式有利于保证数据库的安全。

内模式：描述了数据在存储介质上的存储方式即物理结构。

二级映像：在三个模式之间

外模式/模式映像：一个模式对应多个外模式，每个外模式数据库系统都有一个外模式/模式映像，用以定义外模式和模式之间的关系。好处是在模式改变时，外模式可以不变，这样应用程序也不用变，实现了数据的逻辑独立性。

逻辑独立性：应用程序和数据库的逻辑结构相互独立的性质。

模式/内模式映像：数据的全局逻辑结构和存储结构之间的对应，它是唯一的。当存储结构变化是，修改映像即可，避免了模式的修改，实现了物理独立性。

物理独立性：应用程序和存储结构独立的性质。

