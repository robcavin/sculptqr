from django.db import models

# Create your models here.
class QRCode(models.Model) :
    source_image = models.ImageField(upload_to='images',height_field='source_image_height',width_field='source_image_width')
    source_image_width = models.IntegerField(editable=False)
    source_image_height = models.IntegerField(editable=False)

    cropped_image = models.ImageField(upload_to='images',height_field='cropped_image_height',width_field='cropped_image_width', null=True)
    cropped_image_width = models.IntegerField(editable=False, null=True)
    cropped_image_height = models.IntegerField(editable=False, null=True)

    qr_image = models.ImageField(upload_to='images',height_field='qr_image_height',width_field='qr_image_width', null=True)
    qr_image_width = models.IntegerField(editable=False, null=True)
    qr_image_height = models.IntegerField(editable=False, null=True)
    
    # removed for now since not used    
    #description = models.CharField(max_length=64,blank=True)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
