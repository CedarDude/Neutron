$commitMessage = Read-Host "Enter commit message"
git add .

git commit -m "$commitMessage"

$branch = Read-Host "Enter branch name (default: main)"
if (-not $branch) { $branch = "main" }

$unsafe = Read-Host "Unsafe? (y/n)"
if ($unsafe -eq 'y') {
    git push --force origin $branch
} else {
    git push origin $branch
}
