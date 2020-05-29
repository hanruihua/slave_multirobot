from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

setup_args = generate_distutils_setup(
    packages=['ekf_localization'],
    package_dir={'':'msgs_lib'}
)

setup(**setup_args)
