# Be sure to run Set-ExecutionPolicy -ExecutionPolicy RemoteSigned
# if you have not done so already

# List available USB devices
Write-Output "Listing available USB devices..."
usbipd list

# Prompt user for the bus ID of the device to attach
$busId = Read-Host -Prompt "Enter the bus ID of the device you want to attach (e.g., 1-7)"

# Validate input
if ([string]::IsNullOrWhiteSpace($busId)) {
    Write-Output "No bus ID provided. Exiting."
    exit 1
}

# Attach the selected device
Write-Output "Attaching device with bus ID $busId..."
try {
    & usbipd bind --busid 2-4
    & usbipd attach --wsl --busid=$busId
    Write-Output "Device attached successfully."
} catch {
    Write-Output "Failed to attach device."
}

# After this script is finished running, go to your WSL2 instance
# and run ls /dev/ttyUSB*

# by default it is ls: cannot access '/dev/ttyUSB*': No such file or directory
# after running this script, it should show /dev/ttyUSB#