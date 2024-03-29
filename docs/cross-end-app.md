# Linux 桌面的问题及解决方案

- 面向未来，自上而下
- 解放思想，开源协作

魏永明  
2022-11

		
## 最终用户的反馈

- @机器学习42：  
驱动、适配各种麻烦。命令麻烦。国产机贵一倍，性能差，大部分专业软件无法使用。感觉和诺基亚的塞班系统一样，会被淘汰。
- @HHH-----——82135  
民用版，我下过xxxx，马上放弃了。1、无线鼠标和键盘，直接无法用了。2、网银不能转账。3、打印机连接不了。这还没开启 wps 轻办公，就劝退了。
- @Igor Ljubuncic  
不幸的是，现在很多数量的产品（开源软件）被重新设计或开发，只是为了使软件开发人员更加轻松，而不是为了（最终）用户。很大程度上，Gnome 3、PulseAudio、Systemd 和 Wayland 都没有遵循提高用户体验的宗旨……

		
## Linux 桌面为何“越来越差”

- 欧美需求乏力
  - 欧美厂商没有发展 Linux 桌面的需求
  - 开源的桌面系统技术，由开源爱好者的自嗨驱动
- 国内厂商的跟随策略
  - 仍未摆脱使用现成的开源技术“攒”系统的策略
  - 技术方面乏善可陈
  - 产品规划混乱，难找到亮点

		
## 国产桌面系统的隐忧

1. 仍以模仿 Windows 或 macOS 为主，除了满足信创的基本需求之外，产品无亮点，在开放市场几乎没有竞争力。
1. 应用开发门槛高，兼容性问题突出，开发工具不友好；和互联网行业相差一代。
1. 核心技术全部来源于欧美主导的开源技术，广泛使用商业公司开发的 Qt 库（Qt 并不是免费的）。

		
## 发展国产桌面系统面临的挑战

- 受惠于政策保护，但也可能受政策波动的影响。
- 还没学会走，但形势逼着厂商要跑起来；这可能带来严重的后坐力。
- 卖许可的模式，适合信创产业，但不适合开放市场，企业要未雨绸缪。
- 资金看起来不缺了，但人才匮乏，或者说人才成长得太慢。
   1. 产品的定义无鲜明亮点，战略定力不够，看起来像个大杂烩。
   1. 无法判定开源技术的好坏，盲目追新现象普遍存在。
   1. 更缺乏开发新技术的架构、设计和领导能力。
- 建议从互联网公司引入优秀的产品经理及架构师。

		
## 如何破局？

- 面向未来，自上而下
- 解放思想，开源协作

面对一个细分市场，开发关键应用，同时服务好第三方开发者，逐步向其他市场领域和人群渗透，是发展自主桌面系统的必由之路

		
## 看看其他桌面系统的成长之道

- 从服务内容创作者起步再外溢到其他领域，如苹果的 macOS。macOS 的成功得益于：
   1. 早期阶段，稳稳抓住了内容创作者这个群体：打牢基础。
   1. 苹果作为移动互联网开创者构造的先进生态：面向未来。
- 类似地，谷歌的 ChromeOS 从中小学教育领域起步。

		
## 看未来：桌面系统的发展趋势

- 向云端集中，逐渐与云端融合
   1. 多场景协同，借助云或者边缘设备与各种联网设备进行数据交换。
   1. 基于 Web 的应用越来越流行，大量应用以小程序的形式在浏览器中运行。
- 多极世界格局下，信息安全的需求普遍存在。
   1. 国产桌面系统有可能参与国际竞争。
   1. 但未来桌面操作系统将免费提供。
- 科技战会导致先进技术不可用，开源技术虽然尚未受限，但大概率也会受到影响。

		
## 看未来：应用形态的发展趋势

1. 面向云计算和边缘计算，要满足跨端运行和多场景协同的需求。
1. 让一个应用支持多种操作系统，只是最低的要求。
1. 分布式部署，甚至可在不同设备上迁移，这要求将桌面、智能手机以及各种周边设备（手表、电视、车载、AR/VR 等），纳入到统一的应用框架和分布式服务架构当中进行管理。
1. 这一趋势对编程语言、应用框架、应用的开发、测试和部署，会有极大的影响。

		
## 工作组长期目标

1. 着眼长远，通过开源协作，发展自主的开源公共核心技术，构建一个开源的桌面操作系统底座，降低桌面系统厂商的重复投入成本，让厂商集中精力打磨产品，发展关键应用。
1. 通过 COPU 的开源高地，吸引国际人才和开源社区，加快技术的开发和生态的建设。
1. 通过良好的协作关系，减低重复投入成本的同时，帮助厂商培养技术人才。

		
## 抓手：构建一个开源的桌面系统底座

1. 扔掉历史包袱，换道超车。
1. 从面向未来的编程语言、应用框架入手，优先解决应用开发门槛高、效率低的问题。
1. 从上而下，构建桌面系统的开源基础软件供应链体系。

		
## 具体措施

1. 建议围绕 HVML 发展自主的应用编程语言和应用框架，以快速解决现有技术方案存在的问题。
2. 建议将 HVML 作为重点项目，推荐给 Linux 基金会、GNOME 基金会或者 W3C。
3. 请工作组成员推荐其他优秀的公共开源技术，如玲珑包？
4. 成立跨企业的虚拟开发团队，用于开发公共开源技术。

		
## HVML 主要特色

1. 跨操作系统，可支持桌面应用、嵌入式应用为代表的本地应用的开发。
2. 程序逻辑和渲染交互解耦，可统一 CLI 和 GUI 的开发。
3. 站在巨人的肩膀上：充分利用最先进的 Web 前端技术。
4. 可形成多种应用框架，支持多种应用运行形式（如云应用或远程应用），支持跨端运行。

		
## 工作组下一步规划

1. 由 HVML 社区阐述 HVML 的设计理念、优势及现状。
2. 撰写用于和 Linux 基金会等交流的长期规划和技术方案。

		
# 意见或建议

