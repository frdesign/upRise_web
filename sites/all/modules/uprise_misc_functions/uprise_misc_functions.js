
(function ($) {
  // Jquery onload function.
  $(document).ready(function(){
    
    
    //Change "Read more" link to Play video for video nodes
    $('.node-video li.node-readmore a').text('> Watch video');
    
    // Make videos blue in homepage
    $('.node-teaser.node-video').parent().parent().addClass('tb-wall-black-background') // tb-wall-double-style
    
    //Remove colorbox functionality on teasers
    $('.node-teaser a').removeClass('colorbox-load init-colorbox-load-processed cboxElement');
    
    // Hide the first link which is the user provider url to force users to "read more" before leaving site
    $('.node-web-link.node-teaser div.field-type-text-with-summary a').hide();
    
    // Deactivate links in web-link content type when
    $('.node-web-link.node-teaser div.field-type-text-with-summary div.opengraph-filter a').replaceWith(function() {
        return this.innerHTML;
    });
    
    // Open Web Links in new window
    $('.node-web-link div.field-type-text-with-summary a').attr('target','_blank');
    
   });
})(jQuery); 