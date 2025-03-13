# **Linux Device Driver Development for Raspberry Pi 4B (6.6.74+rpt-rpi-v8)**  

This project focuses on developing and testing **Linux kernel modules** for GPIO control on the **Raspberry Pi 4B**.  

## **Setup**  

### **1. Install Dependencies**  
```sh
sudo apt update && sudo apt upgrade -y
sudo apt install -y raspberrypi-kernel-headers build-essential gpiod
```

### **2. Build & Load Module**  
```sh
make
sudo insmod driver.ko
dmesg | tail -n 10  # Check logs
```

### **3. Unload Module**  
```sh
sudo rmmod driver.ko
```

## **Finding GPIO Offsets**  

### **List GPIO Chips:**  
```sh
gpiodetect
```

### **Find GPIO Offset:**  
```sh
cd /sys/class/gpio/
ls
cat /sys/class/gpio/*chip*/label
```

