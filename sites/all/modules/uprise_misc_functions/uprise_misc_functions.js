
(function ($) {
  // Jquery onload function.
  $(document).ready(function(){
    
    
    //Change "Read more" link to Play video for video nodes
    $('.node-video li.node-readmore a').text('Watch video');
    
    //Change "Read more" link to Rate for quotes nodes
    $('.node-quotes li.node-readmore a').text('Rate');
    
    // Make videos black in homepage
    $('.node-teaser.node-video').parent().parent().addClass('tb-wall-blue-background') // tb-wall-double-style
    
    // Make quotes blue in homepage
    $('.node-teaser.node-quotes').parent().parent().addClass('tb-wall-black-background') // tb-wall-double-style
    
    //Remove colorbox functionality on teasers
    $('.node-teaser a').removeClass('colorbox-load init-colorbox-load-processed cboxElement');
    
    // Hide the first link which is the user provider url to force users to "read more" before leaving site
    $('.node-web-link.node-teaser div.field-type-text a').hide();
    
    // Deactivate links in web-link content type when
    $('.node-web-link.node-teaser div.field-type-text div.opengraph-filter a').replaceWith(function() {
        return this.innerHTML;
    });
    
    // Open Web Links in new window
    $('.node-web-link div.field-type-text-with-summary a').attr('target','_blank');
    
    // Turn add content links in node type page into buttons
    $('dl.node-type-list a').addClass('typo-tag tag-rounded tag-black');
    
    // Try to open Manin Menu links to add nodes on colorbox
    $('.sf-main-menu .sf-item-3 a').addClass('colorbox-load');
    
    // Change profile "view" tabl to just "profile"
    $('body.logged-in.page-user ul.tabs li:first a').text('Profile');
    
    // Change profile "track" tab to just "Posts"
    $('body.logged-in.page-user ul.tabs li:nth-child(4) a').text('Posts');
    
   });
})(jQuery); 