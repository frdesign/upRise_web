<?php

/**
 * Callback function for admin/config/user-interface/qtip
 */
function qtip_settings_form() {
  $settings = _qtip_fetch_cdn_settings();
  $form = array();

  $form['intro'] = array(
    '#markup' => t('These settings determine how the qTip library will be setup on the site.'),
  );

  /* FIELDSETS */
  $form['library'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('qTip library options'),
    '#collapsible' => FALSE,
    '#weight'      => 0,
  );
  $form['library']['cdn_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('CDN Settings'),
    '#collapsible' => FALSE,
    '#weight'      => 1,
    '#states'      => array(
      'invisible' => array(
        ':input[name="qtip_cdn"]' => array('value' => 'none'),
      ),
    ),
  );
  $form['advanced'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#weight'      => 1,
  );

  /* LIBRARY SECTION */
  $form['library']['qtip_cdn'] = array(
    '#type'    => 'select',
    '#title'   => t('qTip CDN'),
    '#description' => t('Choose the CDN to serve the qTip library from. Choose %none to use a custom build.', array('%none' => 'none')),
    '#options' => array(
      'none'     => t('None (custom build)'),
      'jsdelivr' => t('jsDelivr (preferred)'),
      'cdnjs'    => t('CDNJS'),
    ),
    '#default_value' => $settings['qtip_cdn'],
    '#weight'        => 0,
  );
  $form['library']['qtip_library_notice'] = array(
    '#type' => 'item', // Have to use item since markup type will not work with #states
    '#markup' => '<strong>' . t('Place your custom build of the qTip library in sites/all/libraries/qtip.') . '</strong><br>' .
                  t('This setting depends on the !libraries module.', array('!libraries' => '<a href="http://drupal.org/project/libraries">Libraries</a>')),
    '#states' => array(
      'visible' => array(
        ':input[name="qtip_cdn"]' => array('value' => 'none'),
      ),
    ),
  );

  /* CDN SETTINGS SECTION */
  $form['library']['cdn_settings']['qtip_cdn_version'] = array(
    '#type'  => 'select',
    '#title' => t('qTip Version'),
    '#description' => t('Select the verion of qTip to use.'),
    '#options' => array(
      '2.2.0' => '2.2.0 (recommended)',
    ),
    '#default_value' => $settings['qtip_cdn_version'],
  );
  $form['library']['cdn_settings']['qtip_cdn_features'] = array(
    '#type'    => 'radios',
    '#title'   => t('qTip features'),
    '#options' => array(
      'all'   => t('All features & styles'),
      'basic' => t('Basic features (no additional features or styles)'),
    ),
    '#default_value' => $settings['qtip_cdn_features'],
  );
  $form['library']['cdn_settings']['qtip_cdn_compression'] = array(
    '#type'    => 'radios',
    '#title'   => t('Compression level'),
    '#options' => array(
      'min'  => t('Production (minified)'),
      'full' => t('Development (uncompressed)'),
    ),
    '#default_value' => $settings['qtip_cdn_compression'],
  );

  /* ADVANCED SECTION */
  $form['advanced']['qtip_load_every_page'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Load the qTip library on every page'),
    '#description'   => t('<strong>NOT FUNCTIONAL!</strong> If unchecked the qTip library will need to be loaded programmatically with <em>drupal_add_library(\'qtip\', \'qtip\')</em> in your theme or module.<br /><strong>Leave enabled unless you have a specific reason for disabling it.</strong>'),
    '#default_value' => variable_get('qtip_load_every_page', TRUE),
    '#disabled'      => 'disabled',
  );

  return system_settings_form($form);
}
