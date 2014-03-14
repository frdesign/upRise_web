(function ($) {

Drupal.TBThemeFeed = Drupal.TBThemeFeed || {};


Drupal.TBThemeFeed.saveAndFeed = function() {
  $('input[name="tb_theme_feed_manually"]').val(1);
}

})(jQuery);
