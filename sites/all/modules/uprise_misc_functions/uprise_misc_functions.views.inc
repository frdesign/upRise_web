
<?php
function uprise_misc_functions_views_query_alter(&$view, &$query) {
  if ($view->name == 'user_cloud') {
    dsm($query, 'before');
    $query->where[0]['type'] = 'OR';
    dsm($query, 'after');
    
    
  }
}

