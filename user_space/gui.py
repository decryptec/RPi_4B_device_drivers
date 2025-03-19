import tkinter as tk
from tkinter import messagebox

def write_to_driver():
    file_path = driver_path_entry.get()
    message = write_entry.get()
    if var_newline.get():
        message += "\n"
    
    try:
        with open(file_path, 'w') as driver_handle:
            driver_handle.write(message)
        messagebox.showinfo("Success", f"Written: {message}")
    except Exception as e:
        messagebox.showerror("Error", str(e))

def read_from_driver():
    file_path = driver_path_entry.get()
    try:
        with open(file_path, 'r') as driver_handle:
            data = driver_handle.read()
        text_output.config(state="normal")
        text_output.delete("1.0", tk.END)
        text_output.insert(tk.END, data)
        text_output.config(state="disabled")
    except Exception as e:
        messagebox.showerror("Error", str(e))

# GUI Setup
root = tk.Tk()
root.title("ProcFS Driver Interface")

# Device Driver Path Input
tk.Label(root, text="Driver File Path:").grid(row=0, column=0, padx=5, pady=5, sticky="e")
driver_path_entry = tk.Entry(root, width=40)
driver_path_entry.grid(row=0, column=1, padx=5, pady=5)
driver_path_entry.insert(0, "/proc/my_driver")

# Write Section
tk.Label(root, text="Write to Driver:").grid(row=1, column=0, padx=5, pady=5, sticky="e")
write_entry = tk.Entry(root, width=40)
write_entry.grid(row=1, column=1, padx=5, pady=5)
var_newline = tk.IntVar()
tk.Checkbutton(root, text="Add \n", variable=var_newline).grid(row=1, column=2, padx=5)
tk.Button(root, text="Write", command=write_to_driver).grid(row=2, column=1, pady=5)

# Read Section
tk.Button(root, text="Read", command=read_from_driver).grid(row=3, column=1, pady=5)
text_output = tk.Text(root, height=4, width=50)
text_output.grid(row=4, column=0, columnspan=3, padx=5, pady=5)
text_output.insert(tk.END, "Read output here")
text_output.config(state="disabled")

# Exit Button
tk.Button(root, text="Exit", command=root.destroy).grid(row=5, column=1, pady=10)

root.mainloop()