<?php

/**
 * Implementation of hook_views_default_views()
 */
function tb_theme_feed_views_default_views() {
  $view = new view;
  $view->name = 'tb_theme_feed';
  //........................
  $views[$view->name] = $view;
  return $views;
}
