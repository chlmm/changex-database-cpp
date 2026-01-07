# Qt Redis Plus Plus Project

A C++ application using Qt 5.12.12 and redis-plus-plus library for Redis operations.

## Requirements

- C++17 or later
- Qt 5.12.12 or later
- CMake 3.14 or later
- redis-plus-plus library
- A running Redis server

## Project Structure

```
/workspace/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── include/
│   └── redismanager.h     # Redis manager header file
└── src/
    ├── main.cpp            # Application entry point with demo
    └── redismanager.cpp    # Redis manager implementation
```

## Installing Dependencies

### 1. Install Qt 5.12.12

Download and install Qt 5.12.12 from the official Qt website:
https://download.qt.io/archive/qt/5.12/5.12.12/

Or use your package manager:
```bash
# Ubuntu/Debian
sudo apt-get install qt5-default qtbase5-dev

# macOS (using Homebrew)
brew install qt@5

# Windows
# Download and install the online installer from Qt website
```

### 2. Install redis-plus-plus

```bash
# Clone the repository
git clone https://github.com/sewenew/redis-plus-plus.git
cd redis-plus-plus

# Build and install
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

### 3. Start Redis Server

```bash
# Ubuntu/Debian
sudo apt-get install redis-server
sudo service redis-server start

# macOS
brew install redis
brew services start redis

# Windows
# Download and install Redis for Windows or use Docker
docker run -d -p 6379:6379 redis:latest
```

## Building the Project

1. Set Qt 5.12.12 environment variables:

```bash
# Example for Linux/macOS
export Qt5_DIR=/path/to/Qt/5.12.12/gcc_64/lib/cmake/Qt5
export CMAKE_PREFIX_PATH=/path/to/Qt/5.12.12/gcc_64
```

2. Create a build directory and run CMake:

```bash
mkdir build
cd build
cmake ..
```

3. Build the project:

```bash
cmake --build .
# or
make -j$(nproc)
```

## Running the Application

Make sure Redis server is running before executing the application:

```bash
# Check if Redis is running
redis-cli ping
# Expected output: PONG

# Run the application
./QtRedisProject
```

## Features

The `RedisManager` class provides a convenient C++ interface for Redis operations:

### String Operations
- `set(key, value)` - Set a key-value pair
- `get(key)` - Get value by key

### Hash Operations
- `hSet(key, field, value)` - Set hash field
- `hGet(key, field)` - Get hash field value

### List Operations
- `lPush(key, value)` - Push value to list (left)
- `lPop(key)` - Pop value from list (left)

### Set Operations
- `sAdd(key, value)` - Add value to set
- `sIsMember(key, value)` - Check if value is a member

### Generic Operations
- `del(key)` - Delete a key
- `exists(key)` - Check if key exists
- `keys(pattern)` - Get all keys matching pattern

## Example Output

```
Qt Redis Plus Plus Demo
=======================
Connected to Redis server: "127.0.0.1" : 6379

--- String Operations ---
SET "test_key" = "Hello, Redis Plus Plus!"
GET "test_key" = "Hello, Redis Plus Plus!"
Retrieved value: "Hello, Redis Plus Plus!"

--- Hash Operations ---
HSET "user:1" "name" = "Alice"
HSET "user:1" "age" = "30"
HGET "user:1" "name" = "Alice"
HGET "user:1" "age" = "30"
User name: "Alice" , age: "30"

--- List Operations ---
LPUSH "mylist" "item1"
LPUSH "mylist" "item2"
LPUSH "mylist" "item3"
LPOP "mylist" = "item3"
Popped item: "item3"

--- Set Operations ---
SADD "myset" "member1"
SADD "myset" "member2"
SADD "myset" "member3"
SISMEMBER "myset" "member2" = true
Is member2 in set? true

--- Generic Operations ---
EXISTS "test_key" = true
Does 'test_key' exist? true
KEYS "*" found 4 keys
  - "test_key"
  - "user:1"
  - "mylist"
  - "myset"

--- Cleanup ---
DEL "test_key"
DEL "user:1"
DEL "mylist"
DEL "myset"

Demo completed successfully!
```

## Troubleshooting

### CMake cannot find Qt5
Make sure to set `Qt5_DIR` and `CMAKE_PREFIX_PATH` environment variables pointing to your Qt 5.12.12 installation.

### Cannot connect to Redis
- Ensure Redis server is running: `redis-cli ping`
- Check if the correct host and port are configured (default: 127.0.0.1:6379)
- Verify Redis configuration allows connections

### Linking errors with redis-plus-plus
Make sure redis-plus-plus is properly installed and CMake can find it:
```bash
cmake .. -DCMAKE_PREFIX_PATH=/usr/local/lib/cmake/redis++
```

## License

This project is provided as-is for educational and development purposes.
