<?php

/**
 * Implements hook_views_data_alter().
 *
 * Register our qTip field.
 */
function qtip_views_views_data_alter(&$data) {
  $data['views']['qtip_views_field'] = array(
    'title' => t('qTip'),
    'help'  => t('Displays a field with a qTip tooltip.'),
    'field' => array(
      'handler' => 'qtip_views_handler_field_tooltip',
    ),
  );
}
