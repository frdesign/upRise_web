(function ($) {
  Drupal.TBWall = Drupal.TBWall || {};
  Drupal.TBWall.columnWidth = 250;
  Drupal.TBWall.sidebarColumnWidth = 250;
  Drupal.TBWall.toolbarHeight = -1;
  Drupal.TBWall.headerHeight = -1;
  Drupal.TBWall.sidebarIScroll = false;
  Drupal.TBWall.popupIScroll = false;
  Drupal.TBWall.supportedScreens = [0.5, 479.5, 719.5, 959.5, 1049.5, 1235.5, 1585.5, 1890.5];
  Drupal.TBWall.screens = ['empty', 'mobile-vertical', 'mobile', 'tablet-vertical', 'tablet', 'normal-screen', 'wide', 'wide-extra', 'hd'];
  Drupal.TBWall.currentScreen = "";
  Drupal.TBWall.currentWidth = -1;
  Drupal.TBWall.IE8 = $.browser.msie && parseInt($.browser.version, 10) === 8;
  Drupal.TBWall.toolbar = false;
  Drupal.TBWall.masonry_container = false;
  Drupal.TBWall.masonry_sidebar_container = false;
  
  Drupal.behaviors.actionTBWall = {
    attach: function (context) {
      $(window).scroll(function() {
        Drupal.TBWall.btnToTop();
        Drupal.TBWall.loadActualImages();
      });
      
      $(window).resize(function(){
        $('body').css({'padding-top': Drupal.TBWall.toolbar ? (Drupal.TBWall.toolbar.height() - (Drupal.TBWall.IE8 ? 10 : 0)) : 0});
        Drupal.TBWall.loadActualImages();
        Drupal.TBWall.updateResponsiveMenu();
        Drupal.TBWall.updateScrollSize();
        Drupal.TBWall.mobilePopup();
        Drupal.TBWall.currentWidth = window.innerWidth ? window.innerWidth : $(window).width();
        Drupal.TBWall.updateMasonryMainWidth(true);
        Drupal.TBWall.updateMasonrySidebarWidth(true);
        Drupal.TBWall.ie8Resize();
        window.setTimeout(function() {
          $('#modalBackdrop').css('width', '100%');
        }, 500);
        var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
        if(windowWidth > Drupal.TBWall.supportedScreens[4]){
          $('#main-wrapper .page-main-inner, #sidebar-first-wrapper .grid-inner').matchHeights();
        }
      });

      $(window).load(function(){
        Drupal.TBWall.mobilePopup();
        Drupal.TBWall.currentWidth = window.innerWidth ? window.innerWidth : $(window).width();
    	Drupal.TBWall.toolbar = $('#toolbar').length ? $("#toolbar") : false;
    	Drupal.TBWall.btnToTop();
        $('#button-btt').smoothScroll();
        Drupal.TBWall.initResponsiveMenu();
        if (Drupal.TBWall.IE8) {
          Drupal.TBWall.initScroll();
          Drupal.TBWall.updateScrollSize(); 
        } else {
          Drupal.TBWall.iScrollInit();
          Drupal.TBWall.updateScrollSize();
        }
        Drupal.TBWall.iScrollPopupInit();
        Drupal.TBWall.initMasonry();
        $("#block-system-main .view .view-content .views-view-grid").imagesLoaded(function() {
          Drupal.TBWall.initLazyload();
          if(Drupal.TBWall.masonry_container) {
            Drupal.TBWall.masonry_container.masonry('reload');
          }
          window.setTimeout(function() {
            Drupal.TBWall.lazyloadFinished = true
            Drupal.TBWall.loadActualImages();
          }, 500);
        });
        $("body.tb-wall-popup-iframe").find("a").attr('target', '_parent');
        Drupal.TBWall.ie8Resize();
      });
    }
  };

  Drupal.TBWall.ie8Resize = function() {
    if(Drupal.TBWall.IE8) {
      current_screen = "";
      for (i = 0; i < Drupal.TBWall.supportedScreens.length; i ++) {
        if (Drupal.TBWall.currentWidth < Drupal.TBWall.supportedScreens[i]) {
          current_screen = Drupal.TBWall.screens[i];
          break;
        }
      }
      if (current_screen != Drupal.TBWall.currentScreen) {
        if (current_screen == 'normal-screen') {
          $style = '<style type="text/css" class="normal-screen-ie8">.sharethis-buttons { position: inherit; bottom: 0px; right: auto; left: 0px; margin-bottom: 10px;}'
          $style += ".gallery-slides, .gallery-thumbs { width: auto !important; }</style>";
          $("#page").append($($style));
        }
        else {
          $("#page style.normal-screen-ie8").remove();
        }
        Drupal.TBWall.currentScreen = current_screen;
      }
    }
  }

  Drupal.TBWall.updateMainColumnWidth = function() {
	Drupal.TBWall.masonry_container.addClass('masonry-reload');
    var container_width = Drupal.TBWall.masonry_container.width();
    var number_column = Math.round(container_width / Drupal.TBWall.columnWidth);
    var column_width = Math.floor(container_width / number_column);
    Drupal.TBWall.masonry_container.find('.grid.tb-wall-single-style').css({
      width: column_width + "px"
    });
    Drupal.TBWall.masonry_container.find('.grid.tb-wall-double-style').css({
      width: (column_width * 2) + "px"
    });
    Drupal.TBWall.masonry_container.find('.grid.tb-wall-triple-style').css({
      width: (column_width * 3) + "px"
    });
    Drupal.TBWall.masonry_container.data('basewidth', column_width);
    return column_width;
  }

  Drupal.TBWall.updateMasonryMainWidth = function(reload) {
	if(Drupal.TBWall.masonry_container) {
	  Drupal.TBWall.updateMainColumnWidth();
      if(reload) {
        window.setTimeout(function() {
          Drupal.TBWall.masonry_container.masonry('reload');
        }, 20);
      }
	}
  }

  Drupal.TBWall.updateSidebarColumnWidth = function() {
	Drupal.TBWall.masonry_sidebar_container.addClass('masonry-reload');
    var container_width = Drupal.TBWall.masonry_sidebar_container.width();
    var number_column = Math.round(container_width / Drupal.TBWall.sidebarColumnWidth);
    var column_width = Math.floor(container_width / number_column);
    Drupal.TBWall.masonry_sidebar_container.find('.block, .block.grid-single').css({
      width: column_width + "px"
    });
    Drupal.TBWall.masonry_sidebar_container.find('.block.grid-double').css({
      width: (column_width * 2) + "px"
    });
    Drupal.TBWall.masonry_sidebar_container.find('.block.grid-triple').css({
      width: (column_width * 3) + "px"
    });
    Drupal.TBWall.masonry_sidebar_container.data('basewidth', column_width);
    return column_width;
  }

  Drupal.TBWall.updateMasonrySidebarWidth = function(reload) {
	if(Drupal.TBWall.masonry_sidebar_container) {
	  Drupal.TBWall.updateSidebarColumnWidth();
      if(reload) {
        window.setTimeout(function() {
          Drupal.TBWall.masonry_sidebar_container.masonry('reload');
        }, 20);
      }
	}
  }
  
  Drupal.TBWall.initMasonry = function() {
    var $container = $('#block-system-main .view .view-content .views-view-grid');
    if($container.length) {
      Drupal.TBWall.masonry_container = $container;
      Drupal.TBWall.masonry_container.addClass('masonry-reload');
      options = {
        itemSelector: '.views-col',
        isResizable: false,
        isAnimated: false,
        columnWidth: function() {
          Drupal.TBWall.masonry_container.removeClass('masonry-reload');
    	  var basewidth = Drupal.TBWall.masonry_container.data('basewidth');
    	  if(basewidth == undefined) {
    		return Drupal.TBWall.updateMainColumnWidth();
    	  }
    	  else {
            return basewidth;
    	  }
        }
      };
      Drupal.TBWall.masonry_container.masonry(options);
      Drupal.TBWall.updateMasonryMainWidth(true);
    }
    var $container = $('#sidebar-first-wrapper .region-sidebar-first');
    if($container.length) {
      Drupal.TBWall.masonry_sidebar_container = $container;
      Drupal.TBWall.masonry_sidebar_container.addClass('masonry-reload');
      options = {
        itemSelector: '.block',
        isResizable: false,
        isAnimated: false,
        columnWidth: function() {
          Drupal.TBWall.masonry_sidebar_container.removeClass('masonry-reload');
          var basewidth = Drupal.TBWall.masonry_sidebar_container.data('basewidth');
          if(basewidth == undefined) {
            return Drupal.TBWall.updateSidebarColumnWidth();
          }
      	  else {
            return basewidth;
      	  }
        }
      }
      Drupal.TBWall.masonry_sidebar_container.masonry(options);
      Drupal.TBWall.updateMasonrySidebarWidth(true);
    }
  };

  Drupal.TBWall.mobilePopup = function() {
    var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
    if ((Drupal.TBWall.currentWidth == -1) ||
       (windowWidth - Drupal.TBWall.supportedScreens[2]) * 
       (Drupal.TBWall.currentWidth - Drupal.TBWall.supportedScreens[2]) < 0) {
      if (windowWidth < Drupal.TBWall.supportedScreens[2]) {
        $('#main-wrapper .colorbox-load.init-colorbox-load-processed.cboxElement').each(function() {
          str = $(this).attr('href');
          parts = str.split("?");
          $(this).removeClass('colorbox-load')
            .removeClass('init-colorbox-load-processed')
            .removeClass('cboxElement')
            .addClass('tb-wall-colorbox-iframe')
            .attr('href', parts[0]);
        });
      }
      else {
    	$('#main-wrapper .tb-wall-colorbox-iframe').each(function() {
          $(this).addClass('colorbox-load')
          .addClass('init-colorbox-load-processed')
          .addClass('cboxElement')
          .removeClass('tb-wall-colorbox-iframe')
          .attr('href', $(this).attr('href') + "?tb_wall_retro_iframe=1&width=550&height=600&iframe=true");
    	})
      }
    }    
  }

  Drupal.TBWall.iScrollPopupInit = function() {
    el = $('body.tb-wall-popup-iframe #block-system-main .block-inner');
    if(el.length && !Drupal.TBWall.IE8) {
        el.css({height: $(window).height()});
        Drupal.TBWall.popupIScroll = new iScroll(el[0], {vScroll: true, hScroll: false, vScrollbar: true, hScrollbar: false, scrollbarClass: 'myScrollbar', useTransform: false});
    }
  }

  Drupal.TBWall.iScrollInit = function() {
	if($("#menu-left-inner").length) {
      Drupal.TBWall.sidebarIScroll = new iScroll('menu-left-inner', {vScroll: true, hScroll: false, vScrollbar: true, hScrollbar: false,  scrollbarClass: 'myScrollbar', useTransform: false});
	}
  }

  Drupal.TBWall.initLazyload = function() {
    $('img[data-src]').each(function() {
      if(!$(this).hasClass('loading-icon')) {
        $(this).addClass('loading-icon').parent().append('<img class="lazyloader-icon" style="position: absolute;" src="' + Drupal.TBWall.lazyload_icon + '"/>');
      }
    });
  }

  Drupal.TBWall.loadActualImages = function(){
	if(Drupal.TBWall.lazyloadFinished) {
	  images = $('img[data-src]'); 
      images.each(function(){
        if (Drupal.TBWall.windowView(this) && $(this).attr('data-src')){
    	  Drupal.TBWall.loadImage(this);
          $(this).fadeIn('slow');
        }
      });
	}
  };

  Drupal.TBWall.windowView = function(image){
    var windowHeight = $(window).height(),
        windowWidth  = $(window).width(),
        windowBottom = windowHeight + $(window).scrollTop(),
        imageTop     = $(image).offset().top;
    return windowBottom >= imageTop;
  };

  Drupal.TBWall.loadImage = function(image){
    var img = document.createElement('img');
    var src = $(image).attr('data-src');
    $(image).removeAttr('data-src')
    $(img).attr('src', src);
    $(img).imagesLoaded(function(){
      $(image).attr('src', src);
      $(image).removeClass('loading-icon');
      $(image).parent().find('img.lazyloader-icon').remove();
    });
  };

  Drupal.TBWall.updateScrollSize = function() {
    var left_menu = $('#menu-left-wrapper');
    if (left_menu.length) {
      var logo = $('#logo');
      var window_height = window.innerHeight ? window.innerHeight : $(window).height(); 
      var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
      var height = window_height - logo.height();
      var top = (windowWidth < Drupal.TBWall.supportedScreens[3]) ? ($('#header-wrapper').height()) : 80;
      top += Drupal.TBWall.toolbar ? (Drupal.TBWall.toolbar.height() - (Drupal.TBWall.IE8 ? 10 : 0)) : 0;
      left_menu.css({
        'height': height,
        'top': top
      });
      var width = logo.width() + logo.offset().left;
      left_menu.width(width);
      left_menu.find('.region-menu-left').width(width - 10);
      if(Drupal.TBWall.sidebarIScroll && !Drupal.TBWall.IE8) {
        Drupal.TBWall.sidebarIScroll.refresh();
        Drupal.TBWall.sidebarIScroll.scrollTo(0, 0, 300);
      }      
      else if(Drupal.TBWall.IE8){
        $('#menu-left-inner').mCustomScrollbar("update");
        $('#menu-left-inner').mCustomScrollbar("scrollTo", 0);
      }
    }
  }

  Drupal.TBWall.eventStopPropagation = function(event) {
    if (event.stopPropagation) {
      event.stopPropagation();
    }
    else if (window.event) {
      window.event.cancelBubble = true;
    }
  }

  Drupal.TBWall.updateResponsiveMenu = function(){
    var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
    if ((Drupal.TBWall.currentWidth == -1) ||
       (windowWidth - Drupal.TBWall.supportedScreens[3]) * 
       (Drupal.TBWall.currentWidth - Drupal.TBWall.supportedScreens[3]) < 0) {
      if(windowWidth < Drupal.TBWall.supportedScreens[3]){
        $('#menu-bar-wrapper').css({display: 'none'});
        $('#menu-left-wrapper').css({display: 'none'});
        $('#header .responsive-menu-button').show();
      }
      else{
        $('#header .responsive-menu-button').hide();      
        $('#menu-bar-wrapper').css({display: ''});
        $('#menu-left-wrapper').css({display: ''});
      }
    }
  }

  Drupal.TBWall.btnToTop = function(){
    if($(window).scrollTop()) {
      $('#button-btt').fadeIn(1000);
    }
    else {
      $('#button-btt').fadeOut(1000);
    }
  }

  Drupal.TBWall.initResponsiveMenu = function() {
      Drupal.TBWall.updateResponsiveMenu();
      $('#header .tb-left-menu-button').click(function(e){
        var target = $('#menu-left-wrapper');
        target.css({display: target.css('display') != 'none' ? 'none' : 'block'});
        $('#menu-bar-wrapper').css({display: 'none'});
        $('#header-wrapper').css({position: 'fixed'});
        Drupal.TBWall.eventStopPropagation(e);
        Drupal.TBWall.updateScrollSize();
      });
      $('#header .tb-main-menu-button').click(function(e){
        var target = $('#menu-bar-wrapper');
        $('#menu-left-wrapper').css({display: 'none'});
        if(target.css('display') == 'none') {
          window.scrollTo(0, 0);
          target.css({display: 'block'});
          $('#header-wrapper').css({position: 'relative'});
        }
        else {
          target.css({display: 'none'});
          $('#header-wrapper').css({position: 'fixed'});
        }
        Drupal.TBWall.eventStopPropagation(e);
      });
  };
  Drupal.TBWall.initScroll = function() {
    $("#menu-left-inner").mCustomScrollbar({
        set_width:false,
        set_height: '100%',
        horizontalScroll:false,
        scrollInertia:550,
        scrollEasing:"easeOutCirc",
        mouseWheel:"auto",
        autoDraggerLength:true,
        scrollButtons:{
          enable:false,
          scrollType:"continuous",
          scrollSpeed:20,
          scrollAmount:40
        },
        advanced:{
          updateOnBrowserResize:true,
          updateOnContentResize:false,
          autoExpandHorizontalScroll:false
        },
        callbacks:{
          onScroll:function(){},
          onTotalScroll:function(){},
          onTotalScrollOffset:0
        }
      });
  };
})(jQuery);
