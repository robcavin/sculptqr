<script type="text/javascript">
    var target_name;
    var target_width;
    var target_height; 
    var target_aspect_ratio;
    var template_html;
    var image_template_html;
    var caller = null;
    var real_src_width = null;
    var real_src_height = null;
    
    //$("#image_selector_dialog").modal({show:false});
    
    // Field is the div to which the resultant image preview and hidden field should be added
    // *** The name of this div will be prepended to '_img' to form the hidden input field name ***
    // Role is the role of the image, currently gallery,thumbnail,profile_pic, or icon
    // Target size is the size the cropped image will be scaled to
    function add_image(name, role, target_width_in, target_height_in, target) {
	preview_target = target;
	target_name = name;
	target_width = target_width_in;
	target_height = target_height_in;
	target_aspect_ratio = target_width / target_height;
        
        // Save the unmodified template html.  We'll just reset to this after close to clean things up
	template_html = $("#image_selector_dialog").html();
	image_template_html = $("#full_image_wrapper").html();

        $('#target_size').val('(' + target_width + ',' + target_height + ')');
        $('#crop_preview').css({width:target_width, height:target_height});
        /*$("#image_selector_dialog").modal("show").css({
            width: 'auto',
            'margin-left': function () {
                return -($(this).width() / 2); // auto center the modal
            }
        });*/
        $('#upload_role').val(role);
        $("#upload_selector").change(function() {
            $("#upload_form").submit();
        })

        // Called when an image is finished uploading and response is posted to hidden iframe
        $("#upload_frame").load(function() {
            var head = $(this).contents().find("head").html();
            results = eval('(' + $(this).contents().find("body").html() + ')');
            if (image = results['uploaded_image']) {
                $("#full_image").attr('src',image['url']);
                $("#cropped_image").attr('src',image['url']);
                $("#crop_form").attr('action','/qr_code/' + image['id'] + '/image_crop/'); // Note that 'image[id]' is really qr_code id in this instantiation
                real_src_width = image['width'];
                real_src_height = image['height'];
                $("#full_image").Jcrop({
                    aspectRatio:target_aspect_ratio,
                    onSelect:showPreview,
                    onChange:showPreview,
                    bgColor:'blue'
                });    
            }
            else if (image = results['cropped_image']) {
                var results = "";
        // XXX this same code is replicated in the views for forms where images are used and thus will be needed to change in both locations
		results += '<img class= "image_uploader_cropped_image" src="' + image['url'] + '">';
		results += '<a href="#" class="image_uploader_remove_image deletelink" onclick="$(this.parentNode).remove();return false;">Remove</a>'
		results += '<input type="hidden" name="' + target_name + '" value="' + image['id'] + '">'
		$(preview_target).html(results);
                //$("#image_selector_dialog").modal("hide");
            }
        })
    }	

    function showPreview(coords) {
	var width = $("#full_image").width();
	var height = $("#full_image").height();

        var max_dim = (width > height) ? width : height;
        var scale = (max_dim > 400) ? (max_dim * 1.0 / 400) : 1.0;
        //alert(width + ',' + height + ',' + scale);
	var rx = target_width / (scale * coords.w);
	var ry = target_height / (scale * coords.h);
        
	var new_x = Math.round(rx * scale * coords.x);
	var new_y = Math.round(ry * scale * coords.y);
	var new_width = Math.round(rx * width);
	var new_height = Math.round(ry * height);
	$('#cropped_image').css({
		width: new_width + 'px',
		height: new_height + 'px',
		marginLeft: '-' + new_x + 'px',
		marginTop: '-' + new_y + 'px'
	});

        // Update scale to reflect real image height, which might not be the CSS influenced height
        max_dim = (real_src_width > real_src_height) ? real_src_width : real_src_height;
        scale = (max_dim > 400) ? (max_dim * 1.0 / 400) : 1.0;
	$('#crop_rect').val("(" + Math.round(scale * coords.x) + "," + Math.round(scale * coords.y) +
                            "," + Math.round(scale * coords.x2) + "," + Math.round(scale * coords.y2) + ")");
    }
    // XXX note: due to various rounding issues this will sometimes result in off by one pixel images due to differences in aspect ratios.  maybe worth looking into

    /*$("#upload_selector").click(function() {
        //$.Jcrop("#full_image").destroy(); // XXX This should work but seesms to instead completely kill jcrop for all time 
	$("#full_image_wrapper").html(image_template_html);
    });

    $("#image_selector_dialog").on("hidden", function(event, ui) {
	$("#image_selector_dialog").html(template_html);
    })*/

</script>
