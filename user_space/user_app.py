def main():
    proc_file_path = '/proc/my_driver'

    # Writing to the proc file
    message_to_kernel_space = "Hello from user space!"
    with open(proc_file_path, 'w') as driver_handle:
        driver_handle.write(message_to_kernel_space)
        print(f"Written to kernel space: {message_to_kernel_space}")

    # Reading from the proc file
    with open(proc_file_path, 'r') as driver_handle:
        message_from_kernel_space = driver_handle.read()
        print(f"Read from kernel space: {message_from_kernel_space}")

    return 0

if __name__ == "__main__":
    main()

