<?php

/**
 * @file
 * TBDrupalFeed handles feed from drupal.org
 */
class TBDrupalFeed extends TBThemeFeed {

  function __construct($cfg = NULL) {
    $this->configs = $cfg;
    $this->configs['limit'] = $cfg['feed_limit'];
  }

  function getType() {
    return "Drupal";
  }

  public function getContent() {
    $themes_url = "http://drupal.org/project/themes";
    $results = array();
    $page = 0;
    while (true) {
      $url = $themes_url . ($page ? "?page=$page" : "");
      $rows = $this->getData($url);
      foreach ($rows as $row) {
        $results[] = $row;
        if ($this->configs['limit'] != 0 && count($results) >= $this->configs['limit']) {
          return $results;
        }
      }
      if (!count($rows)) {
        break;
      }
      $page++;
    }
    return $results;
  }

  private function getData($url) {
    $url_info = parse_url($url);
    $content = $this->crawlPage($url);
    $dom = new DOMDocument();
    @$dom->loadHTML($content);
    $xdoc = new DOMXPath($dom);
    $themes_containers = $xdoc->evaluate('//*[@id="project-overview"]');
    $themes_container = $themes_containers->item(0);
    $themes = $themes_container->childNodes;
    $rows = array();
    for ($i = 0; $i < $themes->length; $i++) {
      $row = array();
      $theme = $themes->item($i);
      $elements = $theme->childNodes;
      $first_element = $elements->item(0);
      $images = $first_element->getElementsByTagName('img');
      if ($images->length) {
        $image = $images->item(0);
        $row['field_image'] = $this->convertLink("http://" . $url_info['host'], $url, $image->getAttribute('src'));
      }
      $titles = $theme->getElementsByTagName('h2');
      $title = $titles->item(0);
      $row['title'] = $title->nodeValue;
      $links = $theme->getElementsByTagName('a');
      $link = $links->item(0);
      $row['reference_url'] = $row['field_reference_url'] = $this->convertLink("http://" . $url_info['host'], $url, $link->getAttribute('href'));
      $href = $link->getAttribute('href');
      $parts = explode("/", $href);
      $row['feed_id'] = $parts[count($parts) - 1];
      $ps = $theme->getElementsByTagName('p');
      $p = $ps->item(0);
      $row['body'] = $p->nodeValue;
      $row['url'] = $url;
      $row['field_theme_feed_types'] = $this->getType();
      $row['node_type'] = $this->getBundle();
      $row['service'] = 'drupal';
      $row['account'] = '';
      $ul = $theme->getElementsByTagName('ul');
      if ($ul->length) {
        $ul = $ul->item(0);
        $ems = $ul->getElementsByTagName('em');
        $em = $ems->item(0);
        $row['created'] = strtotime($em->nodeValue);
        $row['created'] = empty($row['created']) ? 0 : $row['created'];
        $lis = $ul->getElementsByTagName('li');
        $li = $lis->item(1);
        $row['changed'] = strtotime($li->nodeValue);
        $row['changed'] = empty($row['changed']) ? 0 : $row['changed'];
      }
      $rows[] = $row;
    }
    return $rows;
  }

}
