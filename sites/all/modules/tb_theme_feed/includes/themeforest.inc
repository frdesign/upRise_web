<?php

/**
 * @file
 * TBThemeForestFeed handles feed from http://themeforest.net/feeds/new-cms-themes-slash-drupal-items.atom
 */
class TBThemeForestFeed extends TBThemeFeed {

  function __construct($cfg = NULL) {
    $this->configs = array(
      'limit' => ($cfg['feed_limit'] == 0 || $cfg['feed_limit'] > 100) ? 100 : $cfg['feed_limit']
    );
  }

  function getType() {
    return "ThemeForest";
  }

  public function getContent() {
    $fullurl = 'http://themeforest.net/feeds/new-cms-themes-slash-drupal-items.atom';
    $rss_item_tags = array('item', 'entry');
    $rss_tags = array(
      'title' => array('title'),
      'description' => array('description', 'content', 'summary'),
      'link' => array('link', 'feedburner'),
      'category' => array('category'),
      'updated' => array('updated'),
      'published' => array('published')
    );
    $content = $this->rssToArray($rss_item_tags, $rss_tags, $fullurl);
    $results = array();
    $limit = (count($content) > $this->configs['limit']) ? $this->configs['limit'] : count($content);
    for ($i = 0; $i < $limit; $i++) {
      $row = $content[$i];
      $result = array();
      $result['title'] = $row['title'][0];
      $result['body'] = $row['description'][0];
      $result['field_theme_feed_types'] = $this->getType();
      $result['node_type'] = $this->getBundle();
      $result['feed_id'] = $result['field_reference_url'] = $result['reference_url'] = $row['link'][0];
      $result['created'] = strtotime($row['published'][0]);
      $result['changed'] = strtotime($row['updated'][0]);
      $result['url'] = $fullurl;
      $result['service'] = 'themeforest';
      $result['account'] = '';
      $results[] = $result;
    }
    return $results;
  }

}
