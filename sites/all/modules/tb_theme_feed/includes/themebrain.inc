<?php

/**
 * @file
 * TBThemeBrainFeed handles feed from http://www.themebrain.com/themebrain-feed
 */
class TBThemeBrainFeed extends TBThemeFeed {

  function __construct($cfg = NULL) {
    $this->configs = array(
      'limit' => ($cfg['feed_limit'] == 0 || $cfg['feed_limit'] > 100) ? 100 : $cfg['feed_limit']
    );
  }

  function getType() {
    return "ThemeBrain";
  }

  public function getContent() {
    $fullurl = 'http://www.themebrain.com/themebrain-feed';
    $rss_item_tags = array('item', 'entry');
    $rss_tags = array(
      'title' => array('title'),
      'description' => array('description', 'content', 'summary'),
      'link' => array('link', 'feedburner'),
      'category' => array('category'),
      'pubDate' => array('pubDate')
    );
    $content = $this->rssToArray($rss_item_tags, $rss_tags, $fullurl);
    $results = array();
    $limit = (count($content) > $this->configs['limit']) ? $this->configs['limit'] : count($content);
    for ($i = 0; $i < $limit; $i++) {
      $row = $content[$i];
      $result = array();
      $result['title'] = $row['title'][0];
      $body = $row['description'][0];
      $pattern = '/<img[^>]+src\s*=\s*"([^"]+)"/i';
      preg_match_all($pattern, $body, $matches);
      if (isset($matches[1][0])) {
        $result['field_image'] = $matches[1][0];
        // remove the image from content
        $body = preg_replace('/<img[^>]+src\s*=\s*"([^"]+)".*alt="\s*" *\/>/i', '', $body);
      }
      $result['body'] = $body;
      $result['field_theme_feed_types'] = $this->getType();
      $result['node_type'] = $this->getBundle();
      $result['feed_id'] = $result['field_reference_url'] = $result['reference_url'] = $row['link'][0];
      $result['created'] = $result['changed'] = strtotime($row['pubDate'][0]);
      $result['url'] = $fullurl;
      $result['service'] = 'themebrain';
      $result['account'] = '';
      $results[] = $result;
    }
    return $results;
  }

}
