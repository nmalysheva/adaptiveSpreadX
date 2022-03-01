# algo - README

- Build With CMake (Build Types - Documentation - Tests - Coverage - Static Code Analysis - Dynamic Code Analysis)
- Run
- [Configuration File](#config)
 - Ignored Lines
 - Section Header
 - Settings (Integral - Float - Distribution - Word)
 - Required Sections (Species - Time)
 - Optional Sections (Output - Nodes - Edges - Births - Deaths - Transitions - Interactions)

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
This will start the `section` with the name `abc`.
All following lines (that are not ignored or start a new section) will be interpret as `Settings` for this section.

See below for valid sections.

**Note:** Headers are case-sensitive.

**Note:** Each `section header` must not appear more than once.

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
- `X` is the ID of the distribution, with the `F_i`s being the `Float` parameters.
Supported distributions:
 - `U(a,b)` will create an `uniform distribution` drawing from the range `[a, b]`.
 - `N(m,s)` will create a `normal distribution` with mean `m` and standard deviation `s`. **Note:** If `m+/-0.675s` is not in range `[0, 1]` an exception is thrown. (50% of the drawn values are not in range `[0, 1]`.)
 - `E(l)` will create an `exponential distribution` with rate lambda `l`.
**Note:** `N` and `E` will draw random numbers until the result is in the range `[0, 1]`.

**Note:** The form `X(F_1,F_2,...,F_n)` must not contain whitespaces!

#### Word
A `Word` is a sequence of non-whitespace characters.

### Required Sections
#### Algorithm
Set up the algorithm.

**Syntax:** 

    [Algorithm]
    use Word
    time Float
    output Integer
    epsilon Float

required parameters:
- `time` must be a `Float` value, which determines the how long the algorithm runs
optional parameters:
- `use` sets the algorithm: `SSA` (default) or `SSATAN-X` (not yet implemented)
- `output` output the state of the network every `10^n` steps. If not set only the initial and final state are written.
- `epsilon` used for `SSATAN-X'. If missing a default value is used. Has no effect for `SSA`.

**Example:**

    [Algorithm]
    # use SSATAN-X algorithm
    use SSATAN-X
    # for 100 time units
    time 100
    # output every step
    output 0
    # with custom epsilon
    epsilon 0.25

#### Network
Set up the initial network and set the used states.

**Syntax:**
    [Network]
    Word_1 Integer_1
    Word_2 Integer_2
    ...
    Word_n Integer_n
    edges Integer_e
    seed Integer_s

Each `Word_i` is the name of a state, with `Integer_i` the number of nodes present in the initial network.

**Note:** all states that are used in the simulation must be defined here. If no initial node is desired `0` must be set.

`edges` determines the number of randomly created edges. If missing no edge is created, if larger than the maximum number of edges all nodes are connected.

`seed` is an optional parameter that is used as seed for the random number generator to create the initial set of edges.
If `Integer_s` is not set a random number as seed is taken.

**Example:**

    [Network]
    # create 5 nodes with state A
    S 5
    # and 1 B
    B 1
    # no C, but needed for transitions
    C 0
    # and randomly connect 2 nodes
    edges 1

### Optional Sections

#### Births / Deaths
Rules for creating and removing nodes.

**Syntax:**

    [Births] or [Deaths]
    Word_1 Distribution_1
    Word_2 Distribution_2
    ...
    Word_n Distribution_n

`[Births]`:
- create a node with state `Word_i` with a rate drawn from `Distribtuion_i`
- the rates are drawn on start-up and do not change during simulation
`[Deaths]`:
- delete a node with state `Word_i` with a rate drawn from `Distribtuion_i`
- the rate is drawn for each node individually on creation

**Note:** The used states must be present in section `[Network]`.

**Example:**

    [Births]
    # create S with a fixed propability
    S 0.1
    # and I with a propability drawn uniformly
    I U(0.2,0.4)

    [Deaths]
    # remove only nodes with state I with a fixed propability
    I 0.9

#### AddEdges / RemoveEdges
Rules for creating and removing edges.

**Syntax:**

    [AddEdges] or [RemoveEdge]
    Word_1 Distribution_1
    Word_2 Distribution_2
    ...
    Word_n Distribution_n

Create / delete a connection between two nodes. 
The rates are drawn on node creation.
The rate for the action is calculated by `rate_a * rate_b`.

**Note:** The used states must be present in section `[Network]`.

**Example:**

    [AddEdges]
    # S creates edges on a high rate
    S 0.8
    # I with a low rate
    I 0.1

    [RemoveEdges]
    # S keeps its contacts, so not listed here
    # but I drops connections on a high rate
    I 0.9


#### Transitions
Rule for randomly changing the state of a node.

**Syntax:**

    [Transitions]
    # Word Word Distribution
    F T r

A `Transition` will turn a node of state `F` to a node of state `T` with rate `r`.
Basically this is the same as removing a node and creating a new one, but keeping all connections.

`r` is drawn for each node on creation.

**Note:** The used states must be present in section `[Network]`.

**Example:**

    [Transitions]
    # A turn to B at rate 0.5
    A B 0.5

    # B turns to A with rate drawn uniformly from range [0, 1]
    B A U(0,1)


#### Interactions
Rules for changing the state of a node based on its connections.

**Syntax:**

    [Interactions]
    # Word Word Word Distribution
    F C T r

Same as [Transitions](#Transitions), but the node with state `F` must have a connection to a node of state `C`.

`r` is drawn for each connection on creation.

**Note:** The used states must be present in section `[Network]`.

**Example:**

    [Interactions]
    # If A connected to B make it C at rate 0.1
    A B C 0.5
    # If B connected to A make it C at rate drawn unfirmly from range [0.1,0.2]
    B A C U(0.1,0.2)


#### Quarantines
Rules for cutting edges / connections if a state changes.

**Syntax:**

    [Quarantines]
    # Word Float
    S v

If any state changes to `S` 

**Note:** `v` must be in the range (0, 1]. If 0 is given, it has no effect and will be ignored. If a value greater 1 is given, it will be treated as 1.

**Example:**

    [Quarantines]
    # If turned to A remove half of its contacts
    A 0.5
    # If turned to B remove all of its contacts (typo intended)
    B 2
    # If turned to C remove 50% of its contacts (Error! This will remove all, see note above)
    C 50
    # If turned to D keep all contacts (this will be ignored)
    D 0


