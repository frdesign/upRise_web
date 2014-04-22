
<?php

// This function allows user cloud to have to contextual filters with OR operator
// Either user being viewed created or she uprised to this cloud
function uprise_misc_functions_views_query_alter(&$view, &$query) {
  if ($view->name == 'user_cloud') {
    //dsm($query, 'before');
    $query->where[0]['type'] = 'OR';
    //dsm($query, 'after');
    
  } elseif ($view->name == 'frontpage') {
    //dsm($query, 'before');
    $query->where[0]['type'] = 'OR';
    //dsm($query, 'after');
    
  }
}

