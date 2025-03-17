import tkinter

def main():
    root = tkinter.Tk(screenName=None, baseName=None, className='myApp', useTk=1)
    
    # Add a title to the window
    root.title("myApp")
    
    # Add a label
    label = tkinter.Label(root, text='Read and Write Interface')
    label.grid(row=0, column=0, columnspan=2, pady=10)
    
    # Check Button
    var1 = tkinter.IntVar()
    tkinter.Checkbutton(root, text='Add \\n', variable=var1).grid(row=1, column=0, columnspan=2, pady=5, sticky="w")
    
    # Add user input fields with labels
    tkinter.Label(root, text='User Input').grid(row=2, column=0, padx=5, pady=5, sticky="e")
    tkinter.Label(root, text='ioctrl').grid(row=3, column=0, padx=5, pady=5, sticky="e")
    
    e1 = tkinter.Entry(root)
    e2 = tkinter.Entry(root)
    e1.grid(row=2, column=1, padx=5, pady=5)
    e2.grid(row=3, column=1, padx=5, pady=5)
    
    # Output read
    T = tkinter.Text(root, height=2, width=30)
    T.grid(row=4, column=0, columnspan=2, padx=5, pady=5)  # Used .grid() instead of .pack()
    T.insert(tkinter.END, "Read from kernel")  # Corrected insertion method
    T.config(state="disabled")
    
    # Add an exit button
    exit_button = tkinter.Button(root, text='Exit', width=25, command=root.destroy)
    exit_button.grid(row=5, column=0, columnspan=2, pady=10)
    
    # Start the main event loop
    root.mainloop()

if __name__ == "__main__":
    main()
