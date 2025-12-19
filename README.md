# 🔌 Digital IC Verifier using Arduino Mega 2560

A **low-cost, reliable, and reconfigurable Digital IC Verifier** designed to automatically test **74xx series logic ICs** using **Arduino Mega 2560**.  
This system eliminates manual IC testing by applying predefined logic patterns and validating outputs in real time through a **4×4 keypad** and **I2C LCD display**.

> 💡 Built for **electronics and computer science laboratories, students, and hardware enthusiasts** who need a fast and dependable IC testing solution.

---

## 🚀 Features

- ✅ Automatic verification of **14-pin and 16-pin ICs**
- 🔬 Checks each circuits present in an IC
- 📟 User interaction via **4×4 membrane keypad**
- 🖥️ Clear status display on **I2C LCD**
- 🔌 Uses **16-pin ZIF socket** for safe and easy IC insertion
- 🧩 **Reconfigurable & extendable** – easily add new ICs
- 💰 **Low-cost** alternative to commercial IC testers
- ⚡ Fast and accurate fault detection

---

## 🧠 Working Principle

1. Insert the IC into the **ZIF socket**
2. Select the IC number using the **keypad**
3. Arduino applies predefined input combinations
4. Output responses are compared with expected logic
5. LCD displays corresponding working status

---

## 🧰 Apparatus Required

- Arduino Mega 2560  
- 4×4 Keypad Membrane  
- LCD I2C Display (16×2)  
- 16-Pin ZIF Socket  
- Jumper Wires  

---

## 🔧 Hardware Setup

### 🔹 ZIF Socket Connections
| ZIF Socket Pin | Arduino Mega Pin |
|----------------|------------------|
| 1 – 16         | 31 – 46          |

> Pin 1 of ZIF socket connects to Pin 31 of the Arduino, Pin 2 to Pin 32, and so on up to Pin 16 → Pin 46.

---

### 🔹 Keypad Connections
| Keypad Pin | Arduino Mega Pin |
|----------------|------------------|
| 1 – 8         | 2 - 9         |

> Pin 1 of Keypad connects to Pin 2 of the Arduino , Pin 2 to Pin 3, and so on up to Pin 8 → Pin 9.

---

### 🔹 I2C LCD Connections
| LCD Pin | Arduino Mega Pin |
|--------|--------------|
| GND    | GND          |
| VCC    | 5V           |
| SDA    | Pin 20       |
| SCL    | Pin 21       |

---

## 🧪 Supported ICs

### 🔹 14-Pin ICs
7404, 7400, 7402, 7408, 7428, 7432, 7486,
74136, 74226, 74266, 7410, 7411, 7412,
7415, 7427, 744025, 744075, 7420, 7421,
7425, 744072, 7482, 7473, 7474

### 🔹 16-Pin ICs
7483, 74283, 7442, 74138, 74147, 74148,
74151, 74153, 74157, 74158, 74184,
7468, 7476, 7478, 74112, 74165,
74174, 74175, 74194, 74279

---

## 🛠️ Utilities & Applications

- 🏫 Electronics laboratory IC testing
- 🎓 Academic projects and practical examinations
- 🧪 Pre-checking ICs before circuit design
- 🏭 Can be adapted for industrial IC verification

---

### ✔ Advantages
- More functional than many market IC testers  
- Portable, scalable, and easy to update  
- Reduces testing time and human error  

---

## ⚠️ Limitations

- Supports **only 14-pin and 16-pin ICs**
- Limited to **basic 74xx series ICs**
- During self-test, if:
  - IC exists in database but pin configuration or logic output mismatches  
  → LCD displays **“IC is NOT Recognizable”**

---

## 🔮 Future Scope

- ➕ Support for higher pin-count ICs
- 🧠 EEPROM-based IC database
- 💻 PC interface for test result logging
- 🏭 Industrial-grade IC support

---

## 💻 Installation

Follow the steps below to clone the repository and upload the code to your Arduino Mega 2560.

### 🔹 Clone the Repository
Open a terminal or command prompt and run:

```bash
git clone https://github.com/your-username/your-repo-name.git
```

### 🔹 Navigate to the Project Directory

```bash
cd your-repo-name
```

### 🔹 Open the Code in Arduino IDE

Launch Arduino IDE

Open the .ino file from the cloned project folder

Select the board:

Tools → Board → Arduino Mega 2560

Select the correct COM port from Tools → Port

### 🔹 Upload the Code

Connect Arduino Mega 2560 to your computer using a USB cable

Click the Upload button

Wait for the message “Done uploading”

🎉 Your Digital IC Verifier is now ready to use.

---

## 📜 License

This project is intended for **educational and academic use**.  
You are free to modify, extend, and improve it.

---

⭐ If you find this project useful, **star the repository** and share it!
