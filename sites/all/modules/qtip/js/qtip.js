(function ($) {
  Drupal.behaviors.qtip = {
    attach: function (context, settings) {
      $(function () {
        var styles = $.parseJSON(settings.qtip);
        $('.qtip-link', context).each(function() { // Call .each() so we can access $(this) in the settings/config
          var tooltip          = $(this).next('.qtip-tooltip');
          var tooltip_instance = (tooltip.data('qtip-instance') != undefined) ? tooltip.data('qtip-instance') : 'default';
          var tooltip_title    = (tooltip.data('qtip-title') != undefined) ? tooltip.data('qtip-title') + '' : ''; // Concatenate an empty string to make sure that the value being passed as the title is a string, otherwise it will not display
          var tooltip_text     =  tooltip.html();
          var tooltip_settings = (styles[tooltip_instance] != undefined) ? styles[tooltip_instance] : '';

          if (tooltip_settings) {
            tooltip_settings['content'] = {
              title: tooltip_title,
              text: tooltip_text,
            };
          }
          else {
            tooltip_settings = {
              content: {
                title: tooltip_title,
                text: tooltip_text,
              }
            }
          }

          $(this).qtip(tooltip_settings);
        });
      });
    }
  };
})(jQuery);
