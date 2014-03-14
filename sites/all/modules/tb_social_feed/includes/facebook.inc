<?php

class TBFacebookFeed extends TBSocialFeed {

  function __construct($cfg = NULL) {
    $this->configs = $cfg;
    $this->configs['type'] = 'feed';
    $this->configs['limit'] = $cfg['feed_limit'];
  }

  function getType() {
    return "Facebook";
  }

  public function getContent() {
    $results = array();
    $url = "http://www.facebook.com/feeds/page.php?id={$this->configs['account']}&format=json&t=" . time();
    $content = $this->crawlPage($url);
    $data = json_decode($content);
    if (!isset($data->entries)) {
      return FALSE;
    }
    $results = array();
    $data = $data->entries;
    $limit = (count($data) > $this->configs['limit']) ? $this->configs['limit'] : count($data);
    for ($i = 0; $i < $limit; $i++) {
      $row = $data[$i];
      $result = array();
      $result['feed_id'] = $row->id;
      $result['account'] = $this->configs['account'];
      $result['service'] = 'facebook';
      $result['title'] = $row->author->name;

      // get facebook image
      $content = $this->updateFacebookImage($row->content);
      $pattern = '/<img[^>]+src\s*=\s*"([^"]+)"/i';
      preg_match_all($pattern, $content, $matches);
      if (isset($matches[1][0])) {
        $result['field_image'] = $matches[1][0];
        // remove the image from content
        $content = preg_replace('/<img[^>]+src\s*=\s*"([^"]+)" +alt="\s*" *\/>/i', '', $content);
      }
      // fix url issues in the facebook feed content
      $pattern = '/href="\/l.php\?u=(http[^\'"]+)"/i';
      preg_match_all($pattern, $content, $matches);
      if (isset($matches[1][0])) {
        $url = $matches[1][0];
        $url_array = explode('&', $url);
        $url = 'href="' . rawurldecode($url_array[0]) . '"';
        $content = str_replace($matches[0][0], $url, $content);
      }

      $result['body'] = $content;
      $result['created'] = strtotime($row->published);
      $result['changed'] = strtotime($row->updated);
      $result['feed_type'] = $this->getFeedType();
      $result['field_social_feed_type'] = $this->getType();
      $result['node_type'] = $this->getBundle();
      $result['url'] = $url;
      $result['field_reference_url'] = $result['reference_url'] = $row->alternate;
      $results[] = $result;
    }
    return $results;
  }

  function createAppForm($service, &$form) {
//    $app_info = unserialize($service->app_info);
//    $form[$service->service . "_app_info[app_id]"] = array(
//        '#type' => 'textfield',
//        '#title' => t("App ID"),
//        '#value' => isset($app_info['app_id']) ? $app_info['app_id'] : "",
//    );
//    $form[$service->service . "_app_info[app_secret]"] = array(
//        '#type' => 'textfield',
//        '#title' => t("App Secret"),
//        '#value' => isset($app_info['app_secret']) ? $app_info['app_secret'] : "",
//    );
  }

  /**
   * 	Replace facebook safe image(small size) with original images( big size ) in it's content
   *
   * 	@param $post string Content of facebook item
   */
  private function updateFacebookImage($post) {
    $pattern = '/http[^\'\"\>\<]+(?:safe_image|app_full_proxy).php\?([^\"\'\>]+)/i';
    preg_match($pattern, $post, $matches);
    if (isset($matches[1])) {
      $params = str_replace('&amp;', '&', $matches[1]);

      $patternUrl = '/\&(?:url|src)=([^\"\'\&]+)/';
      preg_match($patternUrl, $params, $matches2);
      if (isset($matches2[1])) {
        $url = rawurldecode($matches2[1]);
        $post = str_replace($matches[0], $url, $post);
      }
    }
    else {
      $pattern = '/<img[^>]+src\s*=\s*"([^"]+)_s.([^"]+)"/i';
      preg_match($pattern, $post, $rb1);
      if (isset($rb1[1])) {
        $post = str_replace('_s.', '_n.', $post);
      }
    }
    return $post;
  }

  function createAccountForm($account, &$form) {
    $form = array(
      '#type' => 'fieldset',
      '#title' => t("Account")
    );
    $form[$account->service . '_feed_account_name[]'] = array(
      '#type' => 'textfield',
      '#size' => 8,
      '#title' => t("Account Name"),
      '#default_value' => isset($account->account_name) ? $account->account_name : ""
    );
    $form[$account->service . '_feed_account[]'] = array(
      '#type' => 'textfield',
      '#size' => 8,
      '#title' => t("Facebook ID"),
      '#default_value' => isset($account->account) ? $account->account : "",
      '#description' => t("Click <a href='http://findmyfacebookid.com/' target='_blank'>here</a> to find your Facebook ID"),
      '#attributes' => array('class' => array('account')),
    );
    $form[$account->service . '_feed_current_account[]'] = array(
      '#type' => 'hidden',
      '#value' => isset($account->account) ? $account->account : ""
    );
    $form[$account->service . '_feed_account_verify'] = array(
      '#markup' => '<a href="javascript:void(0)" class="form-submit btn-verify-account" onclick="Drupal.TBFeed.verifyAccount(\'http://www.facebook.com/feeds/page.php?format=rss20&id=\', this)">' . t('Check feed') . '</a><br/>',
    );
    $form[$account->service . '_feed_account_delete'] = array(
      '#markup' => '<a href="javascript:void(0)" class="btn btn-delete-account" onclick="Drupal.TBFeed.deleteAccount(this)">' . t('Delete') . '</a>',
    );
  }

}
