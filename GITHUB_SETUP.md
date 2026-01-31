# GitHub Setup Guide

## Step 1: Create GitHub Personal Access Token

1. Go to GitHub Settings: https://github.com/settings/tokens
2. Click "Generate new token" → "Generate new token (classic)"
3. Give it a name: `path-planning-lib`
4. Select scopes:
   - ✅ `repo` (Full control of private repositories)
   - ✅ `admin:repo_hook` (Manage webhooks)
5. Click "Generate token"
6. **Copy the token** (it starts with `ghp_`)

**Important**: Save this token somewhere safe - you won't see it again!

## Step 2: Configure Git with Your Credentials

```bash
# Set your username
git config --global user.name "ATaoAlways18"

# Set your email
git config --global user.email "1090790949@qq.com"

# Set GitHub token (replace with your actual token)
git config --global github.token ghp_YOUR_TOKEN_HERE
```

## Step 3: Create GitHub Repository

### Option A: Using GitHub CLI (Recommended)

```bash
# Install GitHub CLI if not already installed
# Then authenticate:
gh auth login

# Create repository:
gh repo create My_Path_planning --public --source=. --remote=origin
```

### Option B: Using GitHub API (Manual)

```bash
# Replace YOUR_TOKEN with your actual token
curl -X POST \
  -H "Authorization: token ghp_YOUR_TOKEN" \
  -H "Accept: application/vnd.github.v3+json" \
  https://api.github.com/user/repos \
  -d '{"name":"My_Path_planning","description":"Comprehensive C++ path planning library with 12 algorithms","private":false,"auto_init":false}'
```

## Step 4: Push to GitHub

```bash
# Navigate to the library directory
cd /home/ct/.openclaw/workspace/ct/code/sf

# Add remote (if not already added)
git remote add origin https://github.com/ATaoAlways18/My_Path_planning.git

# Push to GitHub
git push -u origin master
```

## Step 5: Verify

Visit: https://github.com/ATaoAlways18/My_Path_planning

You should see all your files!

## Alternative: Manual Upload

If you prefer not to use command line:

1. Go to https://github.com/new
2. Create repository: `My_Path_planning`
3. Clone it locally:
   ```bash
   git clone https://github.com/ATaoAlways18/My_Path_planning.git
   ```
4. Copy all files from `/home/ct/.openclaw/workspace/ct/code/sf/` to the cloned repo
5. Commit and push:
   ```bash
   cd My_Path_planning
   git add .
   git commit -m "Initial commit"
   git push origin master
   ```

## Troubleshooting

### Authentication Issues

If you get authentication errors:

```bash
# Use HTTPS with token
git remote set-url origin https://ghp_YOUR_TOKEN@github.com/ATaoAlways18/My_Path_planning.git

# Or use SSH (if you have SSH keys set up)
git remote set-url origin git@github.com:ATaoAlways18/My_Path_planning.git
```

### Permission Denied

Make sure your token has `repo` scope enabled.

### Repository Already Exists

If the repository already exists:

```bash
# Pull first
git pull origin master

# Then push
git push origin master
```

## After Pushing

1. Add a README.md at the root (if not already there)
2. Consider adding a LICENSE file
3. Add topics to your repository: #cpp #path-planning #robotics #ai
4. Consider creating a release: https://github.com/ATaoAlways18/My_Path_planning/releases

## Security Note

⚠️ **Never commit your token to the repository!**

If you accidentally commit your token:
1. Immediately regenerate it on GitHub
2. Remove it from git history (use `git filter-branch` or `git filter-repo`)
3. Use environment variables in the future:
   ```bash
   export GITHUB_TOKEN=ghp_YOUR_TOKEN
   git config --global github.token $GITHUB_TOKEN
   ```

## Using Environment Variables (Recommended)

For better security, use environment variables:

```bash
# Add to ~/.bashrc or ~/.zshrc
export GITHUB_TOKEN="ghp_YOUR_TOKEN"

# Then use in scripts
git config --global github.token $GITHUB_TOKEN
```

## Complete Setup Script

Save this as `setup_github.sh`:

```bash
#!/bin/bash

# Configuration
USERNAME="ATaoAlways18"
EMAIL="1090790949@qq.com"
REPO_NAME="My_Path_planning"
TOKEN="$1"  # Pass token as first argument

if [ -z "$TOKEN" ]; then
    echo "Usage: ./setup_github.sh YOUR_GITHUB_TOKEN"
    exit 1
fi

# Configure Git
git config --global user.name "$USERNAME"
git config --global user.email "$EMAIL"
git config --global github.token "$TOKEN"

# Navigate to library
cd /home/ct/.openclaw/workspace/ct/code/sf

# Create GitHub repository
curl -X POST \
  -H "Authorization: token $TOKEN" \
  -H "Accept: application/vnd.github.v3+json" \
  https://api.github.com/user/repos \
  -d "{\"name\":\"$REPO_NAME\",\"description\":\"Comprehensive C++ path planning library\",\"private\":false}"

# Add remote
git remote add origin "https://github.com/$USERNAME/$REPO_NAME.git"

# Push
git push -u origin master

echo "✅ Repository created and pushed!"
echo "Visit: https://github.com/$USERNAME/$REPO_NAME"
```

Make it executable: `chmod +x setup_github.sh`

Run it: `./setup_github.sh ghp_YOUR_TOKEN`
