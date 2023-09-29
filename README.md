# adaptiveSpreadX

## Description
The adaptiveSpreadX Python Package is a simulation tool designed for modeling and analyzing spreading dynamics on adaptive contact networks. Leveraging the innovative [SSATAN-X](		https://doi.org/10.1051/mmnp/2022035) algorithm, this package enables users to simulate various spreading processes with adaptability and precision. This project is suitable for researchers, developers, and anyone interested in studying the behavior of spreading dynamics.

This project serves as a Python wrapper for the underlying [C++ application](https://github.com/nmalysheva/adaptiveSpreadX/tree/cpp_package), enabling users to leverage the functionalities of the C++ application using Python. This allows for easier integration, usage, and customization in Python environments. At the moment not the full functionality of the C++ application is available in Python. It will be added as needed based on the vision of the developers and user requests.

## Features

- **Adaptive Contact Networks**: Simulate spreading dynamics on networks that adapt and evolve.
- **SSATAN-X Algorithm**: Utilizes the cutting-edge SSATAN-X algorithm for accurate simulations. More detailed description of the algorithm can be found [here](		https://doi.org/10.1051/mmnp/2022035)
- **Versatile Modeling**: Suitable for a wide range of applications including epidemiology, information dissemination, and more.
  
## Installation

```bash
git clone https://github.com/nmalysheva/adaptiveSpreadX.git
cd adaptiveSpreadX
pip install .
```

## Usage example

```python
import adaptiveSpreadX as asx

# Initialize network and configure simulation parameters
configuration = asx.Configuration("config.txt")
network = asx.Network(configuration)
```
At the moment all required parameters are loaded from the configuration file config.txt. More detailed description of that file is provided [here](https://github.com/nmalysheva/adaptiveSpreadX/tree/cpp_package#configuration-file):

```python
# Run the simulation
result = ssatanx.run(configuration, network)

# Analyze and visualize results:

#define timesteps that want to be visualized
t_start = result.timestamps[0]
t_end = result.timestamps[-1]
timesteps = np.linspace(t_start, t_end, int((t_end - 0.0) / 0.5) + 1)

#plot the amount of species of each state over timesteps
asx.plot_species_count_by_state(result, timesteps)

#plot the amount of new state changes of each state over timesteps
asx.plot_new_state_changes(result, timesteps)
```

<!--- j## Documentation

Detailed documentation is available at [Documentation Link](#).

## Usage Examples

### Example 1: Basic Spreading Simulation

```python
import ssatanx

# Configure and run a basic spreading simulation
network = ssatanx.AdaptiveContactNetwork(parameters)
simulation = ssatanx.Simulation(network, spreading_parameters)
results = simulation.run()

# Analyze results
```

### Example 2: Advanced Network Adaptation

```python
import ssatanx

# Configure network adaptation parameters
adaptation_parameters = {
    # ...
}

# Initialize and run the simulation with network adaptation
network = ssatanx.AdaptiveContactNetwork(adaptation_parameters)
simulation = ssatanx.Simulation(network)
results = simulation.run()

# Analyze results
```
 --->
## Contributing

We welcome contributions to the SSATAN-X Python Package! Please read our [Contribution Guidelines](CONTRIBUTING.md) and [Code of Conduct](CODE_OF_CONDUCT.md) for more information.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

We would like to thank the contributors and the community for their support and guidance in the development of this package.

## Contact

For any queries or suggestions, please feel free to reach out to us at [contact@ssatanx.org](mailto:contact@ssatanx.org).
