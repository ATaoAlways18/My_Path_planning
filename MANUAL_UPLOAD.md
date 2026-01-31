# Manual GitHub Upload Guide

Since we don't have GitHub CLI installed, here's the easiest way to upload your library to GitHub.

## Method 1: Using GitHub Web Interface (Easiest)

### Step 1: Create Repository on GitHub

1. Go to https://github.com/new
2. Fill in the form:
   - **Repository name**: `My_Path_planning`
   - **Description**: Comprehensive C++ path planning library with 12 algorithms
   - **Public**: ✅ Public
   - **Initialize with README**: ❌ Unchecked
3. Click "Create repository"

### Step 2: Download Your Library Files

The library is located at: `/home/ct/.openclaw/workspace/ct/code/sf/`

You need to download all these files to your local machine. Since we're in a remote environment, you can:

**Option A: Download as ZIP**
```bash
# In the library directory
cd /home/ct/.openclaw/workspace/ct/code/sf
zip -r path_planning_lib.zip . -x "*.git*" "build/*"
```

Then download the ZIP file using your browser or file transfer method.

**Option B: Clone and Copy**
```bash
# On your local machine, clone the empty repo
git clone https://github.com/ATaoAlways18/My_Path_planning.git
cd My_Path_planning

# Copy all files from /home/ct/.openclaw/workspace/ct/code/sf/ to this directory
# (Use your preferred method: SCP, SFTP, etc.)
```

### Step 3: Upload Files

1. Extract the ZIP file to the cloned repository folder
2. Or copy all files manually to the cloned repository

### Step 4: Commit and Push

```bash
cd My_Path_planning

# Configure Git (if not already done)
git config --global user.name "ATaoAlways18"
git config --global user.email "1090790949@qq.com"

# Add all files
git add .

# Commit
git commit -m "Initial commit: Complete path planning library

- 12 path planning algorithms (BFS, DFS, Dijkstra, A*, IDA*, D*, D* Lite, LPA*, PRM, RRT, RRT*)
- Comprehensive documentation (README, USAGE, INSTALL, etc.)
- CMake build system
- Test suite with 13 test cases
- Working examples

Algorithms:
1. Uninformed Search: BFS, DFS, Dijkstra
2. Heuristic Search: A*, Best-First, IDA*
3. Dynamic Planning: D*, D* Lite, LPA*
4. Sampling-Based: PRM, RRT, RRT*"

# Push to GitHub
git push -u origin master
```

### Step 5: Verify

Visit: https://github.com/ATaoAlways18/My_Path_planning

You should see all your files!

## Method 2: Using GitHub Desktop (GUI)

### Step 1: Install GitHub Desktop
Download from: https://desktop.github.com/

### Step 2: Clone Repository
1. Open GitHub Desktop
2. File → Clone repository
3. Select "URL"
4. Enter: `https://github.com/ATaoAlways18/My_Path_planning.git`
5. Choose local path
6. Click "Clone"

### Step 3: Copy Files
Copy all files from `/home/ct/.openclaw/workspace/ct/code/sf/` to the cloned repository folder.

### Step 4: Commit and Push
1. GitHub Desktop will show all changed files
2. Write a commit message
3. Click "Commit to master"
4. Click "Push origin"

## Method 3: Using Command Line (If you have local access)

### Step 1: Create Repository
```bash
# On your local machine
cd /path/to/your/local/directory

# Clone the library (if you can access it)
# Or download and extract it

# Create GitHub repo
curl -X POST \
  -H "Authorization: token YOUR_GITHUB_TOKEN" \
  -H "Accept: application/vnd.github.v3+json" \
  https://api.github.com/user/repos \
  -d '{"name":"My_Path_planning","description":"Comprehensive C++ path planning library","private":false}'
```

### Step 2: Push Files
```bash
# Navigate to library directory
cd /path/to/library

# Initialize git (if not already done)
git init
git config user.name "ATaoAlways18"
git config user.email "1090790949@qq.com"

# Add remote
git remote add origin https://github.com/ATaoAlways18/My_Path_planning.git

# Add files
git add .
git commit -m "Initial commit"

# Push
git push -u origin master
```

## Method 4: Using File Upload (Single Step)

If you have all files on your local machine:

1. Go to https://github.com/ATaoAlways18/My_Path_planning
2. Click "Add file" → "Upload files"
3. Drag and drop all files and folders
4. Add commit message
5. Click "Commit changes"

## Important Files to Include

Make sure these files are uploaded:

```
My_Path_planning/
├── CMakeLists.txt
├── README.md
├── USAGE.md
├── INSTALL.md
├── SUMMARY.md
├── QUICK_REFERENCE.md
├── GETTING_STARTED.md
├── PROJECT_COMPLETE.md
├── USAGE_DEMO.cpp
├── .gitignore
├── include/sf/
│   ├── path_planning.h
│   ├── graph.h
│   ├── uninformed_search.h
│   ├── informed_search.h
│   ├── dynamic_search.h
│   └── sampling_based.h
├── src/
│   ├── graph.cpp
│   ├── uninformed_search.cpp
│   ├── informed_search.cpp
│   ├── dynamic_search.cpp
│   └── sampling_based.cpp
├── tests/
│   ├── test.cpp
│   └── comprehensive_test.cpp
└── examples/
    └── simple_example.cpp
```

## After Upload Checklist

- [ ] Repository is public
- [ ] All files are uploaded
- [ ] README.md is visible on main page
- [ ] CMakeLists.txt is present
- [ ] All headers in include/sf/
- [ ] All source files in src/
- [ ] Tests in tests/
- [ ] Examples in examples/

## Troubleshooting

### "Authentication failed"
- Make sure your Personal Access Token has `repo` scope
- Use HTTPS URL with token: `https://YOUR_TOKEN@github.com/username/repo.git`

### "Repository already exists"
- Use `git pull origin master` before pushing
- Or use `git push --force` (careful!)

### "Permission denied"
- Make sure you own the repository
- Check token permissions

### Files not showing
- Make sure you added files: `git add .`
- Check .gitignore doesn't exclude important files

## Quick Reference Commands

```bash
# Configure Git
git config --global user.name "ATaoAlways18"
git config --global user.email "1090790949@qq.com"

# Initialize repository
git init

# Add all files
git add .

# Commit
git commit -m "Initial commit"

# Add remote
git remote add origin https://github.com/ATaoAlways18/My_Path_planning.git

# Push
git push -u origin master
```

## Need Help?

If you encounter any issues:
1. Check the error message carefully
2. Make sure you have the correct token with `repo` scope
3. Verify the repository name is correct
4. Check your internet connection
5. Try using GitHub Desktop for easier management

## Success!

Once uploaded, your repository will be available at:
**https://github.com/ATaoAlways18/My_Path_planning**

You can then:
- Share the link with others
- Add it to your portfolio
- Create releases for versions
- Add documentation
- Collaborate with others
