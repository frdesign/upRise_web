<?php
/**
 * @file
 * Override of preprocess functions.
 */
function tb_wall_retro_preprocess_html(&$vars) {  
  $current_skin = theme_get_setting('skin');
  if (isset($_COOKIE['nucleus_skin'])) {
    $current_skin = $_COOKIE['nucleus_skin'];
  }
  $vars['nucleus_skin_classes'] = !empty($current_skin) ? ($current_skin . "-skin") : "";
  
  if(isset($vars['theme_hook_suggestions'][2]) && $vars['theme_hook_suggestions'][2] == 'html__node__250'){
    $vars['theme_hook_suggestions'][] = 'html__404';
  }
  elseif (count($vars['theme_hook_suggestions']) == 1) {
    if (isset($vars['page']['content']['system_main']['main']['#markup']) &&
            trim($vars['page']['content']['system_main']['main']['#markup']) == t('The requested page "@path" could not be found.', array('@path' => request_uri()))) {
      $vars['theme_hook_suggestions'][] = 'html__404';
    }
  }
  if (isset($_GET['tb_wall_retro_iframe']) && $_GET['tb_wall_retro_iframe'] == 1) {
    $vars['theme_hook_suggestions'][] = 'html__popup_iframe';
  }
}

function tb_wall_retro_preprocess_page(&$vars) {
//  drupal_add_css(base_path().'sites/all/themes/tb_wall_retro/skins/retro/style.css', array('type'=>'external', 'media'=>'all'));
  drupal_add_js("
    Drupal.TBWall = Drupal.TBWall || {};
    Drupal.TBWall.lazyload_icon = '" . base_path() . drupal_get_path('theme', 'tb_wall_retro') . "/images/loading-img.gif';
  ", 'inline');
  if (isset($_GET['tb_wall_retro_iframe']) && $_GET['tb_wall_retro_iframe'] == 1) {
    $vars['theme_hook_suggestions'][] = 'page__popup_iframe';
  }
}

function tb_wall_retro_preprocess_node(&$vars) {
  if($vars['type'] == 'tb_social_feed') {
    if(isset($vars['field_social_feed_type'])) {
  	  foreach ($vars['field_social_feed_type'] as $style) {
  	    if(isset($style['taxonomy_term'])) {
          $vars['classes_array'][] = drupal_strtolower($style['taxonomy_term']->name);
  	    }
  	    else {
  	      foreach($style as $term) {
  	      	if(isset($term['tid'])) {
              $t = taxonomy_term_load($term['tid']);
              $vars['classes_array'][] = drupal_strtolower($t->name);
            }
            else {
              $vars['classes_array'][] = drupal_strtolower($t->name);
            }  	        	
  	      }
  	    }
      }
    }  	
  }

  if($vars['view_mode'] == 'teaser') {
  	if(isset($vars['content']['links']['comment']['#links']['comment-add'])) {
  	  unset($vars['content']['links']['comment']['#links']['comment-add']);
  	}
	else if(isset($vars['content']['links']['comment']['#links']['comment_forbidden'])) {
	  unset($vars['content']['links']['comment']['#links']['comment_forbidden']);
	}
  	if($vars['node']->type == 'tb_rss_feed' || $vars['node']->type == 'tb_social_feed') {
  	  if(isset($vars['content']['links']['node']['#links']['node-readmore'])) {
  	    unset($vars['content']['links']['node']['#links']['node-readmore']);
  	  }
  	}
  }
  $vars['date'] = format_date($vars['node']->created, 'custom', 'd M Y');
  $vars['tb_wall_retro_first_field'] = false;
  foreach($vars['content'] as $key => $field) {
    if (isset($field['#field_type']) 
      && isset($field['#weight']) 
      && ($field['#field_type'] == 'image' || $field['#field_name'] == 'field_vimeo' || $field['#field_name'] == 'field_youtube')) {
      $vars['tb_wall_retro_first_field'] = drupal_render($field);
      unset($vars['content'][$key]);
      break;
    }
  }
  if(variable_get('clean_url', 0)) {
    $node_url = $vars['node_url'];
    $vars['tb_wall_retro_iframe_token'] = (strpos($node_url, "?") !== false) ? "&" : "?";
  }
  
  // process theme style
  $skins = nucleus_get_predefined_param('skins', array('default' => t("Default skin")));
  foreach ($skins as $key => $val) {
    if ($vars['node_url'] == base_path() . 'skins/' . $key && (!isset($_COOKIE['nucleus_skin']) || $_COOKIE['nucleus_skin'] != $key)) {
      setcookie('nucleus_skin', $key, time() + 100000, base_path());
      header('Location: ' . $vars['node_url']);
    }
  }
}

/**
 * Implements hook_css_alter().
 */
function tb_wall_retro_js_alter(&$js) {
  if (isset($js[drupal_get_path('module', 'views_infinite_scroll') . '/js/views_infinite_scroll.js']) &&
     (isset($js[drupal_get_path('module', 'views_infinite_scroll') . '/js/jquery.autopager-1.0.0.js']) || 
      isset($js[libraries_get_path('autopager') .'/jquery.autopager-1.0.0.js']))
  ) {
    drupal_add_js(drupal_get_path('theme', 'tb_wall_retro') . '/js/jquery.autopager-1.0.0.js');
    drupal_add_js(drupal_get_path('theme', 'tb_wall_retro') . '/js/views_infinite_scroll.js');
  }
  if (isset($js[drupal_get_path('module', 'colorbox') . '/js/colorbox_load.js'])) {
    drupal_add_js(drupal_get_path('theme', 'tb_wall_retro') . '/js/colorbox_load.js');
  }
}

function tb_wall_retro_teaser_class_fields() {
  return array(
    'field_social_feed_type' => array(
      'default' => false,
    ),
    'field_tb_wall_style' => array(
      'default' => 'tb-wall-single-style',
    ),
    'field_tb_wall_background' => array(
      'default' => false,
    ),
    'field_tb_wall_badge' => array(
      'default' => false,
    )
  );
}

function tb_wall_retro_get_teaser_class($node) {
  $classes = array(); 
  $fields = tb_wall_retro_teaser_class_fields();
  foreach($fields as $field => $info) {
  	$class = false;
    if(isset($node->$field)) {
      $field_content = $node->$field;
      foreach($field_content as $lang) {
        foreach($lang as $term) {
          if(isset($term['taxonomy_term'])) {
            $class = drupal_strtolower($term['taxonomy_term']->name);
          }
          else {
            $t = taxonomy_term_load($term['tid']);
            if($t) {
              $class = drupal_strtolower($t->name);
            }
          }
        }
      }
    }
    $class = $class ? $class : $info['default'];
    $existed = false;
    if($class) {
      $classes[] = $class;
    }
  }
  return $classes;
}

function tb_wall_retro_preprocess_views_view_grid(&$vars) {
  $view = $vars['view'];
  if (isset($view->style_plugin->row_plugin->nodes)) {
    $nodes = $view->style_plugin->row_plugin->nodes;
    $tb_wall_retro_classes = array();
    $counter = 0;
    foreach($nodes as $node) {
      $row_extend_class = tb_wall_retro_get_teaser_class($node);
      $tb_wall_retro_classes[$counter] = implode(" ", $row_extend_class);
      $counter ++;
    }
  }
  $vars['tb_wall_retro_classes'] = $tb_wall_retro_classes;
}

function tb_wall_retro_preprocess_field(&$vars) {
  if($vars['element']['#field_type'] == 'image' && $vars['element']['#entity_type'] == 'node' && $vars['element']['#view_mode'] == 'teaser') {
    foreach ($vars['items'] as $key => $item) {
      if ($item['#image_style'] == 'tb-wall-dynamic-style') {
        $img_style = false;
        if (isset($vars['element']['#object']->field_tb_wall_style)) {
          foreach ($vars['element']['#object']->field_tb_wall_style as $style) {
            foreach ($style as $lang) {
              foreach ($lang as $term) {
                $t = isset($term->name) ? $term : taxonomy_term_load($term);
                $image_style = image_style_load($t->name);
                if ($image_style) {
                  $img_style = $t->name;
                }
              }
            }
          }
        }
        if ($img_style) {
          $vars['items'][$key]['#image_style'] = $img_style; 
        }
        else {
          $image_style = image_style_load('tb-wall-single-style');
          if ($image_style) {
            $vars['items'][$key]['#image_style'] = 'tb-wall-single-style'; 
          }
        }
      }
    }
  }
}

function tb_wall_retro_views_infinite_scroll_pager(&$variables) {
  $variables['img_path'] = base_path() . drupal_get_path('theme', 'tb_wall_retro') . '/images/ajax-loader.gif' ;
  return theme_views_infinite_scroll_pager($variables);
}

function tb_wall_retro_create_blank_image ($width, $height) {
  static $cache_data = false;
  static $cache_exists = false;
  if (!$cache_data) {
  	$cache = cache_get(__FUNCTION__ . ':tb_wall_retro_lazyload_images');
  	if($cache && isset($cache->data)) {
  	  $cache_data = $cache->data;
  	}
  }
  if ($cache_data) {
  	if (isset($cache_data[$width][$height])) {
  	  if(!$cache_exists) {
  	  	if(file_exists($cache_data[$width][$height]['url'])) {
  	      $cache_exists = true;
          return $cache_data[$width][$height]['url'];
  	  	}
  	  }
  	  else {
        return $cache_data[$width][$height]['url'];
  	  }
    }
  }
  else {
  	$cache_data = array();
  }
  $folder_uri = "public://tb_wall_retro_lazyload_images";
  if (!file_exists($folder_uri)) {
    mkdir($folder_uri, 0777);
  }

  $img = imagecreate($width, $height);
  $background = imagecolorallocate($img, 0, 0, 0);
  imagecolortransparent($img, $background);
  $file_uri = $folder_uri . "/" . $width . "x" . $height . ".png";
  $file_url = file_create_url($file_uri);
  imagepng($img, drupal_realpath($file_uri));
  $cache_data[$width][$height] = array('uri' => $file_uri, 'url' => $file_url);
  cache_set(__FUNCTION__ . ':tb_wall_retro_lazyload_images', $cache_data);
  return $file_url;
}

function tb_wall_retro_get_image_size($path) {
  static $cache_data = false;
  if (!$cache_data) {
  	$cache = cache_get(__FUNCTION__ . ':tb_wall_retro_external_images');
  	if(isset($cache->data)) {
  	  $cache_data = $cache->data;
  	}
  }
  if ($cache_data && isset($cache_data[$path])) {
  	return $cache_data[$path];
  }
  $size = getimagesize($path);
  $cache_data[$path] = $size;
  cache_set(__FUNCTION__ . ':tb_wall_retro_external_images', $cache_data);
  return $size;
	
}

function tb_wall_retro_process_image(&$variables) {
  if (isset($variables['width']) && isset($variables['height'])) {
    $variables['attributes']['data-src'] = file_create_url($variables['path']);
    $file_url = tb_wall_retro_create_blank_image($variables['width'], $variables['height']);
    $variables['path'] = $file_url;
  }
  else {
  	$size = tb_wall_retro_get_image_size($variables['path']);
  	if(isset($size[0]) && isset($size[1])) {
  	  $variables['attributes']['data-src'] = file_create_url($variables['path']);
      $file_url = tb_wall_retro_create_blank_image($size[0], $size[1]);
      $variables['path'] = $file_url;
  	}
  }
}

function tb_wall_retro_image_formatter($variables) {
  $item = $variables['item'];
  $image = array(
    'path' => $item['uri'],
  );

  if (array_key_exists('alt', $item)) {
    $image['alt'] = $item['alt'];
  }

  if (isset($item['attributes'])) {
    $image['attributes'] = $item['attributes'];
  }

  if (isset($item['width']) && isset($item['height'])) {
    $image['width'] = $item['width'];
    $image['height'] = $item['height'];
  }

  // Do not output an empty 'title' attribute.
  if (isset($item['title']) && drupal_strlen($item['title']) > 0) {
    $image['title'] = $item['title'];
  }

  if ($variables['image_style']) {
    $image['style_name'] = $variables['image_style'];
    $output = theme('image_style', $image);
  }
  else {
    $output = theme('image', $image);
  }

  // The link path and link options are both optional, but for the options to be
  // processed, the link path must at least be an empty string.
  if (isset($variables['path']['path'])) {
    $path = $variables['path']['path'];
    $options = isset($variables['path']['options']) ? $variables['path']['options'] : array();
    $options += array('attributes' => array('class' => array()));
    $options['attributes']['class'][] = "colorbox-load";
    // When displaying an image inside a link, the html option must be TRUE.
    $options['html'] = TRUE;
    $output = l($output, $path, $options);
  }

  return $output;
}

/**
 * Theme function for thumbnails.
 */
function tb_wall_retro_youtube_thumbnail($variables) {
  $id = $variables['video_id'];
  $style = $variables['image_style'];

  // Get YouTube settings - TODO is this needed?
  $size = variable_get('youtube_size', '420x315');
  $dimensions = youtube_get_dimensions($size);

  $files = variable_get('file_public_path', conf_path() . '/files');
  $youtube = variable_get('youtube_thumb_dir', 'youtube');
  $dest = $files . '/' . $youtube . '/' . $id . '.png';

  // Check to see if a thumbnail exists locally.
  if (!file_exists($dest)) {
    // Retrieve the image from YouTube.
    if (!youtube_get_remote_image($id)) {
      // Use the remote source if local copy fails.
      $src = youtube_build_remote_image_path($id);
      return theme('image', array('path' => $src));
    }
  }

  if ($style) {
    $uri = 'public://' . $youtube . '/' . $id . '.png';
    $image = theme('image_style', array('style_name' => $style, 'path' => $uri));
  }
  else {
    $path = $files . '/' . $youtube . '/' . $id . '.png';
    $image = theme('image', array('path' => $path));
  }

  // Check if an url path is provided
  if ($variables['image_link'] != NULL) {
    $url_path = $variables['image_link']['path'];
    $options = $variables['image_link']['options'];
    $options += array('attributes' => array('class' => array()));
    $options['attributes']['class'][] = "colorbox-load";
    $image = l($image, $url_path, $options);
  }

  return $image;
}
