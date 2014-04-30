<?php

/**
 * @file
 * Administrative functions for the qTip module.
 */

/**
 * Callback function for admin/config/user-interface/qtip
 */
function qtip_form($form, &$form_state, $formtype, $qt = NULL) {
  if (!isset($qt)) {
    $qt = new stdClass;
  }
  else {
    // Get the settings out of the settings array
    $style    = $qt->settings['style'];
    $position = $qt->settings['position'];
    $show     = $qt->settings['show'];
    $hide     = $qt->settings['hide'];
  }

  // The contents of $qt will either come from the db or from $form_state.
  if (isset($form_state['values']['name'])) {
    $qt = _qtip_convert_form_to_qtips($form_state);
  }

  $form = array('#tree' => TRUE);

  $form['name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Name'),
    '#description'   => t('@TODO'),
    '#default_value' => isset($qt->name) ? $qt->name : '',
    '#weight'        => -9,
    '#required'      => TRUE,
  );

  $form['machine_name'] = array(
    '#type'         => 'machine_name',
    '#description' => t('A unique machine-readable name for this qTip instance. It must only contain lowercase letters, numbers, and underscores. The machine name will be used internally by qTip and will be used in the CSS ID of your qTip set.'),
    '#machine_name' => array(
      'exists' => 'qtip_machine_name_exists', // @TODO
      'source' => array('name'),
    ),
    '#maxlength'    => 32,
    '#weight'      => -8,
  );

  if (!empty($qt->machine_name)) {
    $form['machine_name']['#default_value'] = $qt->machine_name;
    $form['machine_name']['#disabled'] = TRUE;
    $form['machine_name']['#value'] = $qt->machine_name;
  }

  /* Fieldsets */
  $form['style'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Style'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['style']['tip_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Tip settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#weight'      => 100,
    '#parents'     => array('style'),
    '#states'      => array(
      'visible' => array(
        ':input[name="style[tip]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['style']['advanced'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#weight'      => 101,
    '#parents'     => array('style'),
  );
  $form['position'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Position'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['position']['advanced'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#weight'      => 101,
    '#parents'     => array('position'),
  );
  $form['show'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Show'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['show']['advanced'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#weight'      => 100,
    '#parents'     => array('show'),
  );
  $form['hide'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Hide'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['hide']['advanced'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#weight'      => 100,
    '#parents'     => array('hide'),
  );

  /* Styles */
  $form['style']['classes'] = array(
    '#type'          => 'select',
    '#title'         => t('Color Scheme'),
    '#options'       =>   array(
      'Standard Color Schemes' => array(
        ''           => t('Plain (default)'),
        'qtip-cream' => t('Cream'),
        'qtip-light' => t('Light'),
        'qtip-dark'  => t('Dark'),
        'qtip-red'   => t('Red'),
        'qtip-green' => t('Green'),
        'qtip-blue'  => t('Blue'),
      ),
      'Advanced Color Schemes' => array(
        'qtip-bootstrap' => t('Bootstrap'),
        'qtip-tipsy'     => t('Tipsy'),
        'qtip-youtube'   => t('YouTube'),
        'qtip-jtools'    => t('jTools'),
        'qtip-cluetip'   => t('ClueTip'),
        'qtip-tipped'    => t('Tipped'),
      ),
      'qtip-custom' => 'Custom Color Scheme',
    ),
    // '#description'   => $settings['select']['description'],
    '#default_value' => isset($style['classes']) ? $style['classes'] : '',
  );
  $form['style']['classes_custom'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Custom CSS Class'),
    '#description'   => t('The custom CSS class that will be used for all qTips.'),
    // '#default_value' => $settings['custom']['default_value'],
    '#states' => array(
      'visible' => array(
        ':input[name="style[classes]"]' => array('value' => 'qtip-custom'),
      ),
    ),
    '#default_value' => isset($style['classes_custom']) ? $style['classes_custom'] : '',
  );
  $form['style']['shadow'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show shadow under tooltips'),
    '#description'   => t('If checked a shadow will display under each tooltip.') . '<br>' .
                          '<strong>' . t('NOTE: This adds a class to each tooltip, which uses the box-shadow CSS3 property, which is not supported in older browsers.') . '</strong>',
    '#default_value' => isset($style['shadow']) ? $style['shadow'] : 0,
    '#return_value'  => 'qtip-shadow',
  );
  $form['style']['rounded_corners'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show tooltips with rounded corners'),
    '#description'   => t('If checked each tooltip will have rounded corners.') . '<br>' .
                          '<strong>' . t('NOTE: This adds a class to each tooltip, which uses the border-radius CSS3 property, which is not supported in older browsers.') . '</strong>',
    '#default_value' => isset($style['rounded_corners']) ? $style['rounded_corners'] : 0,
    '#return_value'  => 'qtip-rounded',
  );
  $form['style']['tip'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show speech bubble tip'),
    '#description'   => t('If checked each tooltip will have a small speech bubble tip appended to them.'),
    '#default_value' => isset($style['tip']) ? $style['tip'] : 0,
    '#states'        => array(
      'invisible' => array(
        ':input[name="style[color_scheme]"]' => array('value' => 'qtip-youtube'),
      ),
    ),
  );
  $form['style']['tip']['dummy'] = array(
    '#markup' => t('NOT YET IMPLEMENTED!'),
  );
  // @TODO
  // $form['style']['advanced']['def'] = array(
  // );
  // @TODO
  // $form['style']['advanced']['widget'] = array(
  // );
  $form['style']['advanced']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width Override'),
    '#description' => t('This property allows you to override all applied CSS width styles for the tooltip. Can be any valid width CSS value.') . '<br>' .
                      '<strong>' . t('Please note that this does not override max/min width styles! Change those in your CSS file.') . '</strong>',
    '#default_value' => isset($style['width']) ? $style['width'] : '',
  );
  $form['style']['advanced']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height Override'),
    '#description' => t('This property allows you to override all applied CSS height styles for the tooltip. Can be any valid height CSS value.') . '<br>' .
                      '<strong>' . t('Please note that this does not override max/min height styles! Change those in your CSS file.') . '</strong>',
    '#default_value' => isset($style['height']) ? $style['height'] : '',
  );


  /* Position */
    $form['position']['at'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#options' => array(
      'top left'      => 'Top Left Corner',
      'top center'    => 'Top Center',
      'top right'     => 'Top Right Corner',
      'right center'  => 'Right Side',
      ''              => 'Bottom Right Corner', // Default value from qTip library
      'bottom center' => 'Bottom Center',
      'bottom left'   => 'Bottom Left Corner',
      'left center'   => 'Left Side',
    ),
    '#description'   => t('Set where the tooltips should display relative to the link text.'),
    '#default_value' => isset($position['at']) ? $position['at'] : '',
  );
  $form['position']['my'] = array(
    '#type' => 'select',
    '#title' => t('Tooltip Placement'),
    '#options' => array(
      ''              => 'Normal',
      'top left'      => 'Top Left Corner',
      'top center'    => 'Top Center',
      'top right'     => 'Top Right Corner',
      'right center'  => 'Right Side',
      'bottom right'  => 'Bottom Right Corner',
      'bottom center' => 'Bottom Center',
      'bottom left'   => 'Bottom Left Corner',
      'left center'   => 'Left Side',
    ),
    '#description'   => t(''),
    '#default_value' => isset($position['my']) ? $position['my'] : '',
  );
  $form['position']['advanced']['target'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Target'),
    '#description'   => t('HTML element the tooltip will be positioned in relation to. Can also be set to \'mouse\' or the \'event\' (position at target that triggered the tooltip), or an array containing an absolute x/y position on the page.'),
    '#default_value' => isset($position['target']) ? $position['target'] : '',
  );
  $form['position']['advanced']['container'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Container'),
    '#description'   => t('Determines the HTML element which the tooltips will be appended to (e.g. the containing element.)') . '<br>' .
                          '<strong>' . t('NOTE: This should usually be left blank') . '</strong>',
    '#default_value' => isset($position['container']) ? $position['container'] : '',
  );
  $form['position']['advanced']['viewport'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Viewport'),
    '#description'   => t('Determines the viewport used to keep the tooltip visible i.e. the element whose boundaries the tooltip must stay visible within at all times if possible.'),
    '#default_value' => isset($position['viewport']) ? $position['viewport'] : '',
  );
  $form['position']['advanced']['adjust']['method'] = array(
    '#type'    => 'select',
    '#title'   => t('Adjustment Method'),
    '#options' => array(
      'flip'  => t('Flip'),
      ''      => t('Flip invert'), // Default value from qTip library
      'shift' => t('Shift'),
      'none'  => t('None'),
    ),
    '#description'   => t('Determines the viewport used to keep the tooltip visible i.e. the element whose boundaries the tooltip must stay visible within at all times if possible.'),
    '#default_value' => isset($position['adjust']['method']) ? $position['adjust']['method'] : '',
  );
  $form['position']['advanced']['adjust']['x'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Adjust X'),
    '#description'   => t('A positive or negative pixel value by which to offset the tooltip in the horizontal plane e.g. the x-axis. Negative values cause a reduction in the value e.g. moves tooltip to the left.'),
    '#default_value' => isset($position['adjust']['x']) ? $position['adjust']['x'] : '',
  );
  $form['position']['advanced']['adjust']['y'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Adjust Y'),
    '#description'   => t('A positive or negative pixel value by which to offset the tooltip in the vertical plane e.g. the y-axis. Negative values cause a reduction in the value e.g. moves tooltip upwards.'),
    '#default_value' => isset($position['adjust']['y']) ? $position['adjust']['y'] : '',
  );
  $form['position']['advanced']['adjust']['mouse'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Make tooltip follow mouse'),
    '#description'   => t('Determines whether the tooltip follows the mouse when hovering over the show target.'),
    '#default_value' => isset($position['adjust']['mouse']) ? $position['adjust']['mouse'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="position[target]"]' => array('value' => 'mouse'),
      ),
    ),
  );
  $form['position']['advanced']['adjust']['resize'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Make tooltip position adjust when screen is resized'),
    '#description'   => t('Determines if the tooltips position is adjusted when the window is resized.'),
    '#default_value' => isset($position['adjust']['resize']) ? $position['adjust']['resize'] : 1,
  );
  $form['position']['advanced']['adjust']['scroll'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Make tooltip position adjust when screen is resized'),
    '#description'   => t('Determines if the tooltips position is adjusted when the window (or position container) is scrolled.'),
    '#default_value' => isset($position['adjust']['resize']) ? $position['adjust']['resize'] : 1,
  );
  // --- @TODO
  // $form['position']['effect'] array(
  //   '#type'        => 'checkbox',
  //   '#title'       => t('Effect'),
  //   '#description' => t(''),
  // );


  /* Show */
  $form['show']['intro'] = array(
    '#markup' => t('The show settings define what events trigger the tooltip to show on which elements, as well as the initial delay and several other properties.'),
  );
  $form['show']['event'] = array(
    '#type'    => 'checkboxes',
    '#title'   => t('Event'),
    '#options' => array(
      'mouseenter' => t('Hover'),
      'focus'      => t('Focus'),
      'click'      => t('Click'),
    ),
    '#description' => t('@TODO'),
    '#default_value' => isset($show['event']) ? $show['event'] : array('mouseenter'),
  );
  $form['show']['solo'] = array(
    '#type'        => 'checkbox',
    '#title'       => t('Only show one tooltip at a time'),
    '#description' => t('Determines whether or not the tooltip will hide all others when the show event is triggered.'),
    '#default_value' => isset($show['solo']) ? $show['solo'] : 0,
  );
  $form['show']['advanced']['target'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Target'),
    '#description' => t('Defines the HTML element(s) which will trigger your specified show event(s). When left blank, the element that the qTip is associated with will be used.'),
    '#default_value' => isset($show['target']) ? $show['target'] : '',
  );
  $form['show']['advanced']['delay'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Delay'),
    '#description'   => t('Time in milliseconds by which to delay showing the tooltip when the show event is triggered on the show target.') . '<br>' .
                          '<strong>' . t('1000 milliseconds = 1 second') . '</strong>',
    '#default_value' => isset($show['delay']) ? $show['delay'] : '',
  );
  $form['show']['advanced']['ready'] = array(
    '#type'        => 'checkbox',
    '#title'       => t('Show tooltip on page load'),
    '#description' => t('Determines whether or not the tooltip is shown as soon as the page has finished loading. This is useful for tooltips which are created inside event handlers, as without it they won\'t show up immediately.'),
    '#default_value' => isset($show['ready']) ? $show['ready'] : 0,
  );
  // --- @TODO
  // $form['show']['effect'] array(
  //   '#type'        => 'checkbox',
  //   '#title'       => t('Effect'),
  //   '#description' => t(''),
  // );


  /* Hide */
  $form['hide']['intro'] = array(
    '#markup' => t('The hide settings define what events trigger the tooltip to hide on which elements, as well as the initial delay and several other properties.'),
  );
  $form['hide']['event'] = array(
    '#type'    => 'checkboxes',
    '#title'   => t('Event'),
    '#options' => array(
      'mouseleave' => t('Leave'),
      'unfocus'    => t('Unfocus'),
      'click'      => t('Click'),
    ),
    '#description' => t('@TODO'),
    '#default_value' => isset($hide['event']) ? $hide['event'] : array('mouseleave'),
  );
  $form['hide']['fixed'] = array(
    '#type'        => 'checkbox',
    '#title'       => t('Keep tooltip visible when hovered'),
    '#description' => t('When enabled, the tooltip will not hide if moused over, allowing the contents to be clicked and interacted with.'),
    '#default_value' => isset($hide['fixed']) ? $hide['fixed'] : '',
  );
  $form['hide']['advanced']['target'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Target'),
    '#description' => t('Defines the HTML element(s) which will trigger your specified hide event(s). When left blank, the element that the qTip is associated with will be used.'),
    '#default_value' => isset($hide['target']) ? $hide['target'] : '',
  );
  $form['hide']['advanced']['delay'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Delay'),
    '#description'   => t('Time in milliseconds by which to delay hiding the tooltip when the hide event is triggered on the hide target.') . '<br>' .
                          '<strong>' . t('1000 milliseconds = 1 second') . '</strong>',
    '#default_value' => isset($hide['delay']) ? $hide['delay'] : '',
  );
  $form['hide']['advanced']['inactive'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Hide after inactivity'),
    '#description' => t('Time in milliseconds in which the tooltip should be hidden if it remains inactive (e.g. isn\'t interacted with). If left blank, the tooltip will not hide when inactive.') . '<br>' .
                          '<strong>' . t('1000 milliseconds = 1 second') . '</strong>',
    '#default_value' => isset($hide['inactive']) ? $hide['inactive'] : '',
  );
  $form['hide']['advanced']['distance'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Distance'),
    '#description' => t('This setting allows you to determine the distance after which the tooltip hides when the mouse is moved from the point it triggered the tooltip. This is what the regular browser tooltips behave like.'),
    '#default_value' => isset($hide['distance']) ? $hide['distance'] : '',
  );
  // --- @TODO
  // $form['hide']['leave'] array(
  //   '#type'        => 'textfield',
  //   '#title'       => t('Leave'),
  //   '#description' => t(''),
  // );
  // --- @TODO
  // $form['hide']['effect'] array(
  //   '#type'        => 'checkbox',
  //   '#title'       => t('Effect'),
  //   '#description' => t(''),
  // );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation handler.
 */
function qtip_form_validate(&$form, &$form_state) {
// print "<pre>qtip.admin.inc:335: "; print_r($); die("</pre>");
}

/**
 * Submission handler.
 */
function qtip_form_submit(&$form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-actions-submit') {
    $qt = _qtip_convert_form_to_qtips($form_state);
    $exists = qtip_load($qt->machine_name);
    if ($exists && empty($exists->in_code_only)) {
      if (qtip_update($qt)) {
        drupal_set_message(t('The qTip instance %name has been updated.', array('%name' => $qt->name)));
      }
    }
    else {
      if (qtip_save($qt)) {
        drupal_set_message(t('The qTip instance %name has been created.', array('%name' => $qt->name)));
      }
    }

    $form_state['redirect'] = 'admin/config/user-interface/qtip';
  }
}

/**
 * Clone qTip set.
 */
function qtip_clone($qt) {
  unset($qt->machine_name);
  $qt->name = '';

  return drupal_get_form('qtip_form', 'clone', $qt);
}

/**
 * Deletion of qTip instance.
 */
function qtip_delete_instance($form, $form_state, $qt) {
  $form['machine_name'] = array('#type' => 'hidden', '#value' => $qt->machine_name);
  $form['name'] = array('#type' => 'hidden', '#value' => $qt->name);
  return confirm_form($form, t('Are you sure you want to delete the qtip set %name?', array('%name' => $qt->name)), 'admin/config/user-interface/qtip', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for qtip deletion.
 */
function qtip_delete_instance_submit($form, &$form_state) {
  qtip_delete($form_state['values']['machine_name']);
  drupal_set_message(t('The qTip instance %name has been removed.', array('%name' => $form_state['values']['name'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/config/user-interface/qtip';
};

/**
 * Export form for qtip.
 */
function qtip_export_form($form, &$form_state, $qt) {
  ctools_include('export');
  // Generate export code
  $code = ctools_export_object('qtip', $qt, '');

  // Create form
  $form = array();
  $form['export'] = array(
    '#type'          => 'textarea',
    '#default_value' => $code,
    '#rows'          => substr_count($code, "\n") + 1,
    '#resizable'     => FALSE,
    '#description'   => t('Place this code in your module\'s implementation of <code>hook_qtip_default_qtip()</code> to provide it as a default quicktab.'),
  );
  $form['done'] = array(
    '#type'  => 'submit',
    '#value' => t('Done'),
  );
  $form['#redirect'] = 'admin/config/user-interface/qtip';

  return $form;
}

/**
 * Helper function to convert the data on admin form into qtip presentation.
 */
function _qtip_convert_form_to_qtips($form_state) {
  $qt = new stdClass();
  $qt->name = $form_state['values']['name'];

  if (isset($form_state['values']['machine_name'])) {
    $qt->machine_name = $form_state['values']['machine_name'];
  }

  $qt->settings = array(
    'style'    => $form_state['values']['style'],
    'position' => $form_state['values']['position'],
    'show'     => $form_state['values']['show'],
    'hide'     => $form_state['values']['hide'],
  );

  return $qt;
}

