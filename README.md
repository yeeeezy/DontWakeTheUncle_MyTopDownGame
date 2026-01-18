This is a 2.5D casual rhythm-stealth game prototype developed in Unreal Engine using C++ and Blueprints within a single day. Inspiration: Inspired by childhood memories of sneaking out at midnight with my cousin to play video games while trying to remain silent as ghosts.

Game Premise: Players take on the role of a kid trying to reach the computer room late at night. You must avoid "Uncle," who is resting in the top-left bedroom; he is a light sleeper, and any noise will instantly alert him.

Instructions:
Objective: Sneak from the starting point (bottom-right) to the computer room (top-right).
Controls:
Steering: Use your Mouse to control the character's rotation and facing direction.
Space Surge: Watch the rhythm circle at the bottom. Hit 【Space】 exactly when the circle is full. Otherwise, you will make a loud noise and alert your Uncle!

Technical Implements:
Hybrid Input System: Combines keyboard movement with real-time mouse-based rotation for a seamless 2.5D experience.
Dynamic Rhythm Algorithm (C++): Utilizes C++ timers for high-precision rhythm detection.
High-Pressure Logic: Includes a 50% chance to trigger a 0.2s rapid-response challenge, simulating sudden adrenaline spikes.
UI:
Responsive UI: Built with UMG, featuring dynamically scaling rhythm rings and real-time result screens.

Video Demo: https://www.bilibili.com/video/BV15FkWBfENj/?spm_id_from=333.1387.homepage.video_card.click&vd_source=fc1b61aa4b9305d0cc754afb39b35b47

本项目是我使用 虚幻引擎 (Unreal Engine) C++ 结合蓝图，仅用一天时间开发的一款 2.5D 休闲节奏游戏原型。 灵感来源： 灵感源于童年回忆——那时候和表弟为了半夜偷偷起来打游戏，必须像特工一样在走廊屏息潜行。
游戏设定： 玩家扮演一名想在深夜溜进电脑房的孩子。你必须避开正在左上角卧室休息的“舅舅”，他是个觉很轻的人，任何响动都会惊醒他。

游戏说明：
核心目标：从右下角的起点出发，潜行抵达右上角的电脑房。
操作指南：
角色转向：使用鼠标控制角色的实时旋转和朝向。
空格脉冲：观察底部的节奏圆圈，在圆圈全满的瞬间按下【空格】。如果错过时机，会发出巨大噪音惊动舅舅导致失败！

技术实现 ：
混合输入系统：结合了键盘位移与基于鼠标位置的实时角色转向逻辑，确保 2.5D 视角下的流畅操控。
动态节奏判定 (C++)：底层使用 C++ 定时器实现高精度判定。
高压随机逻辑：设计了 50% 的概率触发 0.2s 极速响应模式。

UI：
响应式 UI：基于 UMG 构建，实现了动态缩放的节奏环交互及实时结算界面。

视频链接：https://www.bilibili.com/video/BV15FkWBfENj/?spm_id_from=333.1387.homepage.video_card.click&vd_source=fc1b61aa4b9305d0cc754afb39b35b47
