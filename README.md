# 🌑 Escape the Eclipse

![SFML](https://img.shields.io/badge/SFML-2.5.1-blue)  

**Escape the Eclipse** is a 2D action-adventure game in C++ powered by **SFML**.  
Run through a mysterious desert while an **eclipse unleashes monsters**! Jump over obstacles, survive, and test your reflexes in this endless challenge.  

![Gameplay Preview](resources/images/gameplay_placeholder.gif)  

---

## 🎮 Features
- **Dynamic Player Animations**: Smooth Run & Jump animations  
- **Parallax Backgrounds**: 8-layer scrolling for immersive depth  
- **Random Obstacles**: Never the same run twice  
- **Collision Detection**: Game Over if you hit an obstacle  
- **Restartable Gameplay**: Press **R** to restart after death  


---

## ⌨️ Controls


| Key                    | Action                       |
| ---------------------- | ---------------------------- |
| **Space**              | Jump                         |
| **R**                  | Restart game after Game Over |
| **Esc / Close Window** | Exit game                    |


---

## 💻 Installation & Compilation
Make sure SFML is installed on your system. Compile using `g++`:

```bash
git clone https://github.com/imane-mbarek/GAME_CPP.git
cd GAME_CPP/src
g++ main.cpp Animation.cpp Layer.cpp Obstacle.cpp -o EscapeTheEclipse \
-lsfml-graphics -lsfml-window -lsfml-system
./EscapeTheEclipse```


---



