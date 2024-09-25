# Configurable variables
$foldersToDelete = @('Debug', 'Debug64', 'Release', 'Release64', ".vs")  # Folders to delete
$fileSizeThresholdMB = 5  # Size threshold in MB for files to delete

# Convert size threshold to bytes
$fileSizeThresholdBytes = $fileSizeThresholdMB * 1MB

# Function to safely move items to Recycle Bin
function Move-ToRecycleBin {
    param ([string]$Path)
    
    try {
        # Attempt to move the item to the Recycle Bin using Remove-Item
        Remove-Item -Path $Path -Recurse -Force -ErrorAction Stop
        Write-Host "Successfully moved to Recycle Bin: $Path" -ForegroundColor Green
    } catch {
        # Catch any errors and display a message
        Write-Host "Failed to move to Recycle Bin: $Path - Skipping" -ForegroundColor Red
    }
}

# Initialize counters
$folderCount = 0
$fileCount = 0

# Recursively move specified folders to Recycle Bin, including hidden ones
$folders = Get-ChildItem -Path . -Recurse -Directory -Force | Where-Object {
    # Filter directories based on the names in the $foldersToDelete array
    $foldersToDelete -contains $_.Name
}

if ($folders.Count -eq 0) {
    Write-Host "No folders found to delete." -ForegroundColor Yellow
} else {
    $folders | ForEach-Object {
        # Move the folder to Recycle Bin
        Move-ToRecycleBin -Path $_.FullName
        $folderCount++
    }
}

# Recursively move files larger than the specified size to Recycle Bin, including hidden ones
$files = Get-ChildItem -Path . -Recurse -File -Force | Where-Object {
    # Filter files that are larger than the specified size threshold
    $_.Length -gt $fileSizeThresholdBytes
}

if ($files.Count -eq 0) {
    Write-Host "No files found exceeding the size threshold." -ForegroundColor Yellow
} else {
    $files | ForEach-Object {
        # Move the file to Recycle Bin
        Move-ToRecycleBin -Path $_.FullName
        $fileCount++
    }
}

# Output the summary
Write-Host "`nSummary:"
Write-Host "$folderCount folder(s) moved to Recycle Bin."
Write-Host "$fileCount file(s) moved to Recycle Bin."

# Pause before exiting
Read-Host -Prompt "Press Enter to exit"
