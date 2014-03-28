
(function ($) {
  // Jquery onload function.
  $(document).ready(function(){
    
    //Make node links into buttons
    $(".node-full .node-links a").addClass("typo-btn btn-danger typo-btn-sm btn-sm-danger");
    
    //Make comment reply link into button
    $("li.comment-reply a").addClass("typo-btn btn-danger typo-btn-sm btn-sm-danger");
    
    //Change "Read more" link to Play video for video nodes
    $('.node-video li.node-readmore a').text('Watch video');
    
   });
})(jQuery); 