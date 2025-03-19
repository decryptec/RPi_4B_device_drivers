import tkinter as tk
from tkinter import messagebox
import os
import sys

def write_to_driver():
    file_path = file_path_entry.get()
    message = write_entry.get()
    if var_newline.get():
        message += "\n"
    try:
        with open(file_path, 'w') as driver:
            driver.write(message)
        messagebox.showinfo("Success", "Message written successfully!")
    except Exception as e:
        messagebox.showerror("Error", f"Failed to write: {e}")

def read_from_driver():
    file_path = file_path_entry.get()
    try:
        with open(file_path, 'r') as driver:
            content = driver.read()
        output_text.config(state="normal")
        output_text.delete(1.0, tk.END)
        output_text.insert(tk.END, content)
        output_text.config(state="disabled")
    except Exception as e:
        messagebox.showerror("Error", f"Failed to read: {e}")

def send_ioctl():
    file_path = file_path_entry.get()
    ioctl_command = ioctl_entry.get()

    if sys.platform == "win32":
        messagebox.showerror("Error", "IOCTL is not supported on Windows.")
        return

    import fcntl  # Import only if running on Unix
    try:
        with open(file_path, 'r') as driver:
            fcntl.ioctl(driver.fileno(), int(ioctl_command))
        messagebox.showinfo("Success", "IOCTL command sent successfully!")
    except Exception as e:
        messagebox.showerror("Error", f"IOCTL failed: {e}")

root = tk.Tk()
root.title("Procfs Driver Interface")

tk.Label(root, text="Device File Path").grid(row=0, column=0, padx=5, pady=5, sticky="e")
file_path_entry = tk.Entry(root, width=40)
file_path_entry.grid(row=0, column=1, padx=5, pady=5)

tk.Label(root, text="Write Message").grid(row=1, column=0, padx=5, pady=5, sticky="e")
write_entry = tk.Entry(root, width=40)
write_entry.grid(row=1, column=1, padx=5, pady=5)

var_newline = tk.IntVar()
tk.Checkbutton(root, text='Add \n', variable=var_newline).grid(row=1, column=2, padx=5)

tk.Button(root, text="Write", command=write_to_driver).grid(row=2, column=1, pady=5)

tk.Button(root, text="Read", command=read_from_driver).grid(row=3, column=1, pady=5)

output_text = tk.Text(root, height=5, width=50, state="disabled")
output_text.grid(row=4, column=0, columnspan=3, padx=5, pady=5)

tk.Label(root, text="IOCTL Command").grid(row=5, column=0, padx=5, pady=5, sticky="e")
ioctl_entry = tk.Entry(root, width=20)
ioctl_entry.grid(row=5, column=1, padx=5, pady=5)

tk.Button(root, text="Send IOCTL", command=send_ioctl).grid(row=6, column=1, pady=5)

tk.Button(root, text="Exit", command=root.destroy).grid(row=7, column=1, pady=10)

root.mainloop()
