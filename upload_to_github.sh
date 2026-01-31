#!/bin/bash

# Path Planning Library - GitHub Upload Script
# Usage: ./upload_to_github.sh YOUR_GITHUB_TOKEN

set -e  # Exit on error

# Configuration
USERNAME="ATaoAlways18"
EMAIL="1090790949@qq.com"
REPO_NAME="My_Path_planning"
TOKEN="$1"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if token is provided
if [ -z "$TOKEN" ]; then
    print_error "GitHub token is required!"
    echo ""
    echo "Usage: ./upload_to_github.sh YOUR_GITHUB_TOKEN"
    echo ""
    echo "To get a GitHub token:"
    echo "1. Go to https://github.com/settings/tokens"
    echo "2. Click 'Generate new token' → 'Generate new token (classic)'"
    echo "3. Name it: path-planning-lib"
    echo "4. Select scope: ✅ repo (Full control of private repositories)"
    echo "5. Click 'Generate token'"
    echo "6. Copy the token (starts with ghp_)"
    echo ""
    exit 1
fi

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ] || [ ! -d "include" ] || [ ! -d "src" ]; then
    print_error "Not in the library directory!"
    print_info "Please run this script from: /home/ct/.openclaw/workspace/ct/code/sf"
    exit 1
fi

print_info "Starting GitHub upload process..."
print_info "Username: $USERNAME"
print_info "Repository: $REPO_NAME"
print_info "Email: $EMAIL"

# Step 1: Configure Git
print_info "Step 1: Configuring Git..."
git config --global user.name "$USERNAME"
git config --global user.email "$EMAIL"
git config --global github.token "$TOKEN"
print_success "Git configured"

# Step 2: Check if repository exists
print_info "Step 2: Checking if repository exists..."
REPO_EXISTS=$(curl -s -H "Authorization: token $TOKEN" \
    -H "Accept: application/vnd.github.v3+json" \
    "https://api.github.com/repos/$USERNAME/$REPO_NAME" | \
    grep -q '"message":"Not Found"' && echo "no" || echo "yes")

if [ "$REPO_EXISTS" = "yes" ]; then
    print_warning "Repository already exists!"
    print_info "Adding remote and pushing..."
    
    # Remove existing remote if any
    git remote remove origin 2>/dev/null || true
    
    # Add remote
    git remote add origin "https://$TOKEN@github.com/$USERNAME/$REPO_NAME.git"
    
    # Pull first (if there are any commits)
    git pull origin master --allow-unrelated-histories 2>/dev/null || true
    
    # Push
    git push -u origin master
    
    print_success "Repository updated!"
else
    print_info "Repository doesn't exist, creating it..."
    
    # Create repository
    CREATE_RESPONSE=$(curl -s -X POST \
        -H "Authorization: token $TOKEN" \
        -H "Accept: application/vnd.github.v3+json" \
        https://api.github.com/user/repos \
        -d "{\"name\":\"$REPO_NAME\",\"description\":\"Comprehensive C++ path planning library with 12 algorithms\",\"private\":false,\"auto_init\":false}")
    
    # Check if creation was successful
    if echo "$CREATE_RESPONSE" | grep -q '"name":"'"$REPO_NAME"'"'; then
        print_success "Repository created on GitHub!"
        
        # Add remote
        git remote add origin "https://$TOKEN@github.com/$USERNAME/$REPO_NAME.git"
        
        # Push
        git push -u origin master
        
        print_success "Code pushed to GitHub!"
    else
        print_error "Failed to create repository!"
        echo "Response: $CREATE_RESPONSE"
        exit 1
    fi
fi

# Step 3: Verify
print_info "Step 3: Verifying..."
REPO_URL="https://github.com/$USERNAME/$REPO_NAME"
print_success "Upload complete!"
print_info "Repository URL: $REPO_URL"
print_info "You can now access your repository at:"
echo ""
echo "  $REPO_URL"
echo ""
print_info "Next steps:"
echo "  1. Visit the URL above to see your repository"
echo "  2. Add a README.md if needed"
echo "  3. Consider adding a LICENSE file"
echo "  4. Add topics: #cpp #path-planning #robotics #ai"
echo ""
print_success "Done! 🎉"
