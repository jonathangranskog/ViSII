# Copyright (c) 2020 NVIDIA Corporation. All rights reserved.
# This work is licensed under the NVIDIA Source Code License - Non-commercial. Full
# text can be found in LICENSE.md
from setuptools import setup, dist
import wheel 

# required to geneerate a platlib folder required by audittools
from setuptools.command.install import install
class InstallPlatlib(install):
    def finalize_options(self):
        install.finalize_options(self)
        if self.distribution.has_ext_modules():
            self.install_lib = self.install_platlib

# force setuptools to recognize that this is
# actually a binary distribution
class BinaryDistribution(dist.Distribution):
    def is_pure(self):
        return False
    def has_ext_modules(foo):
        return True

setup(
    # This package is called visii
    name='visii',

    packages = ['visii'], # include the package "visii" 

    # make sure the shared library is included
    package_data = {'': ("*.dll", "*.pyd", "*.so")},
    include_package_data=True,

    description='',

    # See class BinaryDistribution that was defined earlier
    distclass=BinaryDistribution,

    # This gets the version from the most recent git tag, potentially concatinating a commit hash at the end.
    use_scm_version={
        'fallback_version': '0.0.0-dev0',
        "root" : "..",
        "relative_to" : __file__ 
    },

    # Note, below might give "WARNING: The wheel package is not available." if wheel isnt installed
    setup_requires=['setuptools_scm'], # discouraged

    author='Nate Morrical',
    author_email='',
    maintainer='',
    maintainer_email='',
    
    python_requires = ">=2.7",
    cmdclass={'install': InstallPlatlib},
)
