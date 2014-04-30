<?php

/**
 * Page callback to list qTips in the system.
 */
function qtip_list() {
  $header = array(
    array('data' => t('Name')),
    array('data' => t('Machine Name')),
    array('data' => t('Storage')),
    array('data' => t('Operations'), 'colspan' => 4),
  );
  $rows = array();

  foreach (qtip_load_multiple() as $qt) {
    // Determine storage
    switch ($qt->export_type) {
      case EXPORT_IN_DATABASE | EXPORT_IN_CODE:
        $storage = t('Overridden');
        $delete = l(t('Revert'), 'admin/config/user-interface/qtip/manage/'. $qt->machine_name .'/delete');
        break;
      case EXPORT_IN_DATABASE:
        $storage = t('Normal');
        $delete = l(t('Delete'), 'admin/config/user-interface/qtip/manage/'. $qt->machine_name .'/delete');
        break;
      case EXPORT_IN_CODE:
        $storage = t('Default');
        $delete = '';
        break;
    }

    $tablerow = array(
      array('data' => check_plain($qt->name)),
      array('data' => check_plain($qt->machine_name)),
      array('data' => $storage),
      array('data' => l(t('Edit'), 'admin/config/user-interface/qtip/manage/'. $qt->machine_name .'/edit')),
      array('data' => l(t('Export'), 'admin/config/user-interface/qtip/manage/'. $qt->machine_name .'/export')),
      array('data' => l(t('Clone'), 'admin/config/user-interface/qtip/manage/'. $qt->machine_name .'/clone')),
      array('data' => $delete),
    );
    $rows[] = $tablerow;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No qTip instances available.'), 'colspan' => 7));
  }

  $table = theme('table', array('rows' => $rows, 'header' => $header));

  return '<h2 class="label">' . t('qTip instances') . '</h2>' . $table;
}
