# algo - README

- Build With CMake (Build Types - Documentation - Tests - Coverage - Static Code Analysis - Dynamic Code Analysis)
- Run
- [Configuration File](#config)
 - Ignored Lines
 - Section Header
 - Settings (Integral - Float - Distribution - Word)
 - Required Sections (Species - Time)
 - Optional Sections (Nodes - Edges - Births - Deaths - Transitions - Interactions)

## Build
A `CMakeLists.txt` is provided for easy building.

### Build Types
Supported build types are `Debug` (default) and `Release`.


### Documentation
**Option:** `-DBUILD_DOC` (default: **ON**)

**Target:** `doc`

**Requires:** `doxygen`

Output is written to `CMAKE_BINARY_DIR/doc`.


### Tests
**Option:** `-DBUILD_TESTING` (default: **ON**)

**Target:** `test` (or running `ctest`)

**Requires:** `catch` (included in `test/third_party`)


### Coverage
**Option:** `-DBUILD_COVERAGE`

**Target:** `coverage_report`

**Requires:** build type is `Debug`, tools: `lcov`, `genhtml`

Output is written to `CMAKE_BINARY_DIR/test/html/`.


### Static Code Analysis
**Option:** `-DBUILD_STATICANALYSIS` (default: **OFF**)

**Target:** `staticanalysis`

**Requires:** `clang-tidy`


### Dynamic Code Analysis
**Option:** -

**Target:** run `ctest -T MemCheck`

**Requires:** `valgrind`

Output is written to `CMAKE_BINARY_DIR/Testing/Temporary/MemoryChecker.*.log`


## Run
Run with `algo path-to-config-file`.


## Configuration File
The configuration file is a simple text file with `settings` grouped by `section`s.

A line is either a `section header`, a `setting`, or will be `ignored`.

### Ignored Lines
Empty lines or lines containing only whitespaced are ignored.

Lines starting with `#` denote a comment and will also be ignored.

**Note:** `Ignored` lines can be put anywhere.

### Section Header
A `section header` is a line containing `[abc]`.
This will start the `section` `abc`.
All following lines (that are not ignored or start a new section) will be interpret as `Settings` for this section.

See below for valid sections.

**Note:** Headers are case-sensitive.

**Note:** Each section header must not appear more than once.

**Note:** A `section header` must not contain whitespaces.

### Settings
Depending on the format required by the `section` a `setting` is a line of text with whitespace separated `segment`s.
A `segment` can either be `Integral`, `Float`, `Distribution`, or `Word`.


#### Integral
An `Integral` is a non-negative whole number.

#### Float
A `Float` is a non-negative floating point number.

**Note:** Every `Integer` is a valid `Float`.

#### Distribution
A `Distribution` can be expressed in two ways: `F` or `X(F_1,F_2,...,F_n)`.
- `F`: use a fixed `Float` value that will be returned everytime a number is drawn.
- `X` is the ID of the distribution, with `F_i` being the `Float` parameters.
Supported distributions:
 - `U(a,b)` will create a `uniform distribution` drawing from the range `[a, b]`.

**Note:** The form `X(F_1,F_2,...,F_n)` must not contain whitespaces!

#### Word
A `Word` is a sequence of non-whitespace characters.

### Required Sections
#### Species
**Syntax:** 

    [Species]
    # Word Distribution Distribution
    X c r

Add `X` as a species / state to the simulation.
A node with state `X` will gain a new connection with rate drawn from `c` and lose a connection with rate drawn from `r`.

**Example:**

    [Species]
    # state A gets new connections with rate 1 and loses with rate drawn from [0.1, 0.2]
    A 1 U(0.1,0.2)
    # state B can only make new connections (with rate 0.5)
    B 0.5 0

#### Time
**Syntax:** 

    [Time]
    # Float
    f

A simulation starts at time `t_0=0` and finishes at `t_f=f`.

**Example:**

    # Run 10.5 time-units
    [Time]
    10.5

**Note:** If only one simulation step is desired `Time` can be set to 0.
**Example:**

    # Stop after 1 simulation step / event
    [Time]
    0


### Optional Sections

#### Output
**Syntax:**

  [Output]
  # Integer
  n

Output the graph every `10^n-th` step.
**Note:** The first and last step are always printed, even if this entry is missing.

**Example:**

  # Print every 100 step: initial, 100, 200, 300, ..., last
  [Output]
  10

#### Nodes
**Syntax:**

    [Nodes]
    # Word Integer
    X c

Create `c` nodes with state `X`.

**Example:**

    [Nodes]
    # create 10 nodes of state A
    A 10
    # create 1 node of state B
    B 1

#### Edges
**Syntax:**

    [Edges]
    # Integer
    c

Initilise the graph with `c` randomly picked edges.

**Example:**

    # Randomly create 10 edges
    [Edges]
    10

**Note:** If the `Integer` is greater than the possible number of edges, the graph will be fully connected.

#### Births
**Synatx:** 

    [Births]
    # Word Distribution
    S r

A `Birth` will create a new node of state `S` with rate `r`.

The new node will not have any connections to other nodes, but the propability of edge creation and deletion will be drawn as set in [Species](#Species).

**Example:**

    [Births]
    # Create a new node of state X with rate 0.5
    X 0.5
    # Create a new node of state Y with a uniformly distributed range 0.6 +- 0.1
    Y U(0.5,0.7)

**Note:** The rate is drawn again at each simulation step.

#### Deaths
Same as [Births](#Births) but the node with the specified state will be removed from the simulation.
Uses header `[Deaths]`.


#### Transitions
**Syntax:**

    [Transitions]
    # Word Word Distribution
    F T r

A `Transition` will turn a node of state `F` to a node of state `T` with rate `r`.

Basically this is the same as removing a node and creating a new one, but keeping all connections.

**Example:**

    [Transitions]
    # A turn to B at rate 0.5
    A B 0.5

    # B turns to A with rate drawn uniformly from range [0, 1]
    B A U(0,1)


**Note:** The rate is drawn for each node at each simulation step.


#### Interactions
**Syntax:**

    [Interactions]
    # Word Word Word Distribution
    F C T r

Same as [Transitions](#Transitions), but the node with state `F` must have a connection to a node of state `C`.

**Example:**

    [Interactions]
    # If A connected to B make it C at rate 0.1
    A B C 0.5
    # If B connected to A make it C at rate drawn unfirmly from range [0.1,0.2]
    B A C U(0.1,0.2)

**Note:** The rate is drawn for each node at each simulation step.

