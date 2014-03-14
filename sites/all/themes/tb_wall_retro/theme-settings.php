<?php
/**
 * @file
 * Theme setting callbacks for the nucleus theme.
 */
// Impliments hook_form_system_theme_settings_alter().
function tb_wall_retro_form_system_theme_settings_alter(&$form, $form_state) {
  $form['nucleus']['global_settings']['theme_settings']['breadcrumb_display'] = NULL;
}
