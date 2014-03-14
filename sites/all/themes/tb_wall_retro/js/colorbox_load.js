(function ($) {
Drupal.behaviors.initColorboxLoad = {
  attach: function (context, settings) {
    if (!$.isFunction($.colorbox)) {
      return;
    }
    $.urlParams = function (url) {
      var p = {},
          e,
          a = /\+/g,  // Regex for replacing addition symbol with a space
          r = /([^&=]+)=?([^&]*)/g,
          d = function (s) { return decodeURIComponent(s.replace(a, ' ')); },
          q = url.split('?');
      while (e = r.exec(q[1])) {
        e[1] = d(e[1]);
        e[2] = d(e[2]);
        switch (e[2].toLowerCase()) {
          case 'true':
          case 'yes':
            e[2] = true;
            break;
          case 'false':
          case 'no':
            e[2] = false;
            break;
        }
        if (e[1] == 'width') { e[1] = 'innerWidth'; }
        if (e[1] == 'height') { e[1] = 'innerHeight'; }
        p[e[1]] = e[2];
      }
      return p;
    };
    $('a, area, input', context)
      .filter('.colorbox-load')
      .once('init-colorbox-load', function () {
        var href = $(this).attr('href');
    	rel_href = href + (href.indexOf('?') > -1 ? "&" : "?") + 'tb_wall_retro_iframe=1';
    	href += (href.indexOf('?') > -1 ? "&" : "?") + 'iframe=true&width=600&height=600&tb_wall_retro_iframe=1';
        var params = $.urlParams(href);
        $(this).colorbox($.extend({href: rel_href}, settings.colorbox, params));
      });
	$('.node-tb-social-feed .field-name-field-vimeo a', context)
	  .addClass('colorbox-load')
	  .once('init-colorbox-load', function () {
	    href = $(this).attr('href');
	    href += (href.indexOf('?') > -1 ? "&" : "?") + 'iframe=true&width=600&height=600&tb_wall_retro_iframe=1';
	    var params = $.urlParams(href);
	    $(this).colorbox($.extend({href: href}, settings.colorbox, params));
	  });	     
  }
};

})(jQuery);
