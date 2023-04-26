👏 mermaid|Sublime中安装使用mermaid

---
[TOC]

---
## 简介
Mermaid是一个用于**画流程图、状态图、时序图、甘特图**的库，使用JS 进行本地渲染，广泛集成于许多Markdown编辑器中。

## 安装
(1) Ctrl + Shift + p，搜索Install Package并进入。  
(2) 搜索mermaid。  
(3) 进行安装。

## 简单使用
在MarkDown中使用mermaid就像嵌入Python代码一样，将代码头部写好即可自动渲染。如下：  
```mermaid
graph LR
    id1(Start)-->id2(Stop)
    style id1 fill:#f9f,stroke:#333,stroke-width:4px
    style id2 fill:#ccf,stroke:#f66,stroke-width:2px,stroke-dasharray: 5, 5
```