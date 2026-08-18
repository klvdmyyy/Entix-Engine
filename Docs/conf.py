# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Entix Engine'
copyright = '2026, Klementiev Dmitry'
author = 'Klementiev Dmitry'
release = '0.1.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'breathe', # For Doxygen integration
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_clarity_theme'
html_static_path = ['_static']

# -- Breathe configuration ---------------------------------------------------

breathe_projects = {
    'Entix Engine': os.path.abspath('./xml/'),
}

breathe_default_project = 'Entix Engine'
