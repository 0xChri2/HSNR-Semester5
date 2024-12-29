#!/bin/bash
# Exit immediately if a command exits with a non-zero status
set -e

# Variables
CIVICRM_VERSION="5.80.1" # Replace with the desired CiviCRM version
CIVICRM_ZIP="civicrm-${CIVICRM_VERSION}-wordpress.zip"
CIVICRM_URL="https://download.civicrm.org/${CIVICRM_ZIP}"
PLUGIN_DIR="/var/www/html/wp-content/plugins"

# Move to WordPress plugin directory
cd ${PLUGIN_DIR}

# Download CiviCRM
wget ${CIVICRM_URL}

# Unzip the downloaded file
7z x ${CIVICRM_ZIP}

# Remove the zip file
rm ${CIVICRM_ZIP}

# Ensure proper permissions
chown -R www-data:www-data civicrm
chmod -R 755 civicrm

# Print completion message
echo "CiviCRM has been downloaded and unzipped in the WordPress plugins directory."
echo "Next steps: Activate the plugin and run the installer via the WordPress admin panel."