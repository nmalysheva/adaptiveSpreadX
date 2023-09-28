import _adaptiveSpreadX as _asx

try:
    import json
except ImportError as err_msg:
    print("\033[1m adaptiveSpreadX does not install `json` as a dependency. Please install it manually. \033[0m")
    raise err_msg

try:
    import matplotlib.pyplot as plt
except ImportError as err_msg:
    print("\033[1m adaptiveSpreadX does not install `matplotlib` as a dependency. Please install it manually. \033[0m")
    raise err_msg

try:
    import numpy as np
except ImportError as err_msg:
    print("\033[1m adaptiveSpreadX does not install `numpy` as a dependency. Please install it manually. \033[0m")
    raise err_msg
try:
    import warnings
except ImportError as err_msg:
    print("\033[1m adaptiveSpreadX does not install `warnings` as a dependency. Please install it manually. \033[0m")
    raise err_msg


class Result:
    def __init__(self, raw_result):
        data = json.loads(raw_result)
        self.runtime = data.get("runtime")

        #get all states availible
        self.possible_states = set()
        network_config = data.get("configuration", {}).get("Network", [])
        for config in network_config:
            state = config.split()[0]  # The state abbreviation is the first part of the configuration string
            if state != "edges":
                self.possible_states.add(state)
        networks = data.get('networks')
        self.timestamps = []
        self.cumulative_states = {}
        self.new_state_changes = {}
        for s in self.possible_states:
            self.cumulative_states[s] = []
            self.new_state_changes[s] = []

        for entry in networks:
            timestamp = entry.get("simulation_time")
            self.timestamps.append(timestamp)
            current_network_state = entry.get('network')
            nodes = current_network_state.get("nodes")
            for s in self.possible_states:
                nodes_of_state = list(filter(lambda x: x.get("state") == s, nodes))
                self.cumulative_states[s].append(len(nodes_of_state))
                changed_states_newly = list(
                    filter(lambda x: x.get("state") == s and x.get("last_change") == timestamp, nodes))
                self.new_state_changes[s].append(len(changed_states_newly))

        # convert to numpy array. TODO - do it in the beginning?
        self.timestamps = np.array(self.timestamps)
        for s in self.possible_states:
            self.cumulative_states[s] = np.array(self.cumulative_states[s])
            self.new_state_changes[s] = np.array(self.new_state_changes[s])

            self.new_state_changes[s][0] = 0 #no state changes at time 0

def run(config, network):
    print("running simulation...")
    return Result(_asx.simulate(config, network))


def plot_species_count_by_state(result: Result, states_list=[], timesteps = []):
    #if no state list is provided, all states are plotted by default
    if len(states_list) == 0:
        warnings.warn("The list of states is not provided. All available states will be used: ", result.possible_states)
        states_list = result.possible_states
    cumulative_to_plot = dict(filter(lambda item: item[0] in states_list, result.cumulative_states.items()))

    # if no timesteps are provided, all simulation timesteps are plotted by default
    if len(timesteps) == 0:
        warnings.warn("Specific timesteps are not provided. All available timestamps will be used")
        timesteps = result.timestamps
    indices = np.searchsorted(result.timestamps, timesteps, side='right') - 1

    cumulative_to_plot = {key: value[indices] for key, value in cumulative_to_plot.items()}
    for key, value in cumulative_to_plot.items():
        plt.plot(timesteps, value, 'o-', label=key)
    plt.legend(loc="upper right")
    plt.grid(color='lightgrey', linestyle='dashed')
    plt.show()

def plot_new_state_changes(result: Result, states_list=[], timesteps = []):
    #if no state list is provided, all states are plotted by default
    if len(states_list) == 0:
        warnings.warn("The list of states is not provided. All available states will be used : å", result.possible_states)
        states_list = result.possible_states
    new_state_changes_to_plot = dict(filter(lambda item: item[0] in states_list, result.new_state_changes.items()))

    # if no timesteps are provided, all simulation timesteps are plotted by default
    if len(timesteps) == 0:
        warnings.warn("Specific timesteps are not provided. All available timestamps will be used")
        timesteps = result.timestamps
    indices = np.searchsorted(result.timestamps, timesteps, side='right') - 1
    new_state_changes_to_plot = {key: [0] + [sum(value[start + 1:end + 1]) if 0 <= start <= end <= len(value) else None for start, end in
                    zip(indices, indices[1:])] for key, value in new_state_changes_to_plot.items()}
    for key, value in new_state_changes_to_plot.items():
        plt.plot(timesteps, value, 'o-', label=key)
    plt.legend(loc="upper right")
    plt.grid(color='lightgrey', linestyle='dashed')
    plt.show()




