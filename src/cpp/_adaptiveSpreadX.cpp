
// Created by Malysheva, Nadezhda on 09.09.23.
//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <algorithm/Algorithm.hpp>
#include <fstream>
#include <cmath>

namespace py = pybind11;


void bind_Configuration(py::module& m)
{
    py::class_<configuration::Configuration>(m, "Configuration")
            //.def(py::init<std::istream&>())
            .def(py::init([](std::string fname) {
                auto file = std::ifstream{fname}; // NOLINT
                return configuration::Configuration{file};
    }), py::return_value_policy::automatic)
    .def("get", &configuration::Configuration::get)
;}


void bind_Network(py::module& m)
{
    py::class_<network::ContactNetwork>(m, "Network")
    .def(py::init<const configuration::Configuration &>())
    .def("to_json", &network::ContactNetwork::to_json)
 ;}

 void bind_Algorithm(py::module& m)
 {
     m.def("simulate", [](configuration::Configuration const& config, network::ContactNetwork& network) {
         auto json = utils::json::Block{};
         auto algo = algorithm::make_algorithm(config, network);
         if (auto const unused = config.get_unused(); unused)
         {
             throw std::logic_error{std::string{"Unknown section: "} + *unused};
         }

         json.add_json("configuration", config.to_json());auto const start = std::chrono::system_clock::now();
         algo->run(json);
         auto const end = std::chrono::system_clock::now();
         auto const duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();
         json.add_number("runtime", duration);
         return  json.to_string();
     });
 }
PYBIND11_MODULE(_adaptiveSpreadX, m) {
bind_Configuration(m);
bind_Network(m);
bind_Algorithm(m)
;}