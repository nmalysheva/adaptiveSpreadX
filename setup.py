from distutils.core import setup, Extension

extension_modules = Extension(
    '_adaptiveSpreadX',
    #headers=['adaptiveSpreadX/network/AdaptionData.hpp'],
    sources=['src/cpp/configuration/Configuration.cpp',
             'src/cpp/configuration/Exception.cpp',
             'src/cpp/configuration/Stream.cpp',
             'src/cpp/configuration/Helper.cpp',
             'src/cpp/utils/Json.cpp',
             'src/cpp/utils/Parse.cpp',
             'src/cpp/types/Distribution.cpp',
             'src/cpp/types/DistributionImpl.cpp',
             #contact network
             'src/cpp/network/ContactNetwork.cpp',
             'src/cpp/network/IndividualFactory.cpp',
             'src/cpp/network/Interaction.cpp',
             'src/cpp/network/InteractionManager.cpp',
             'src/cpp/network/Settings.cpp',
             'src/cpp/graph/GraphImpl.cpp',
             'src/cpp/types/Propability.cpp', #todo fix the name
             'src/cpp/types/State.cpp',
             'src/cpp/types/NodeId.cpp',
             #algorithm
             'src/cpp/algorithm/ActionsFactory.cpp',
             'src/cpp/algorithm/Algorithm.cpp',
             'src/cpp/algorithm/Settings.cpp',
             'src/cpp/algorithm/SSA.cpp',
             'src/cpp/algorithm/SSATANX.cpp',
             'src/cpp/algorithm/TauLeap.cpp',
             'src/cpp/utils/Random.cpp',
              #pybind11
             'src/cpp/_adaptiveSpreadX.cpp'
            ],
    extra_compile_args=['-std=c++17', '-Wall', '-Wextra',  '-pedantic',  '-Werror'],
    include_dirs=[f"/Library/Developer/CommandLineTools/usr/include/c++/v1", f"/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include", f"src/cpp"],
    #include_dirs=[f"adaptiveSpreadX"],

    language='c++'
)

setup (name = 'adaptiveSpreadX',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [extension_modules])