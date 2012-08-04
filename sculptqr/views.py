# Create your views here.
from django.shortcuts import render, redirect, get_object_or_404
from sculptqr.models import QRCode
from django import forms
from django.http import HttpResponse
from django.conf import settings

# For image upload
import os
import shutil
import Image as PILImage
import ImageFilter
import StringIO
from django.core.files.base import ContentFile
from django.core.files import File

import random
def generated_rnd_base32(length):
    convDict = { 0:'0', 1:'1', 2:'2', 3:'3', 4:'4', 5:'5', 6:'6', 7:'7',
                 8:'8', 9:'9',10:'A',11:'B',12:'C',13:'D',14:'E',15:'F',
                16:'G',17:'H',18:'J',19:'K',20:'M',21:'N',22:'P',23:'Q',
                24:'R',25:'S',26:'T',27:'V',28:'W',29:'X',30:'Y',31:'Z'}
    result = ''
    for dummy in range(length):
        result += convDict[random.randrange(0, 32)]
    return result


def redirect_service(request,code) :
    qr_code = QRCode.objects.get(code=code)
    return redirect(qr_code.url)
    
#-----------------------------------
#
#  IMAGES
#
#-----------------------------------
class ImageUploadForm(forms.Form):
    url = forms.URLField()
    image = forms.ImageField()

def homepage(request, qr_code_id):
    qr_code = None
    if (qr_code_id) : qr_code = QRCode.objects.get(pk=qr_code_id)
    if request.method == 'POST':
        image_form = ImageUploadForm(request.POST,request.FILES)
        if image_form.is_valid():
            file = image_form.cleaned_data['image']
            qr_code = QRCode()
            #image.description = ""
            qr_code.url = image_form.cleaned_data['url']
            qr_code.code = generated_rnd_base32(8)
            
            qr_code.source_image = file
            
            (image_path,file_name) = os.path.split(qr_code.source_image.path)
            (file_name_prefix,file_name_ext) = file_name.rsplit('.',1)
            cropped_image_file_name = file_name_prefix + '_cropped.jpg'

            image_data = PILImage.open(qr_code.source_image)            
            image_data.thumbnail((16,16),PILImage.ANTIALIAS)
            image_data = image_data.convert('1')
            image_io = StringIO.StringIO()
            image_data.save(image_io,'png')
            image_file = ContentFile(image_io.getvalue())
    
            qr_code.cropped_image.save(cropped_image_file_name,image_file)

            qr_image_file_name = 'qr_code_'+ str(qr_code.id) +'.png'
            
            sculptqr_url = 'http://' + request.get_host() + '/c/' + qr_code.code + '/'
            os.system('/server/sculptqr/project/c/encode ' + sculptqr_url + ' ' + settings.MEDIA_ROOT + qr_code.cropped_image.name)
            shutil.move('/tmp/foo.png','/tmp/' + qr_image_file_name);

            image_data = PILImage.open('/tmp/' + qr_image_file_name,'r')
            image_data = image_data.resize((400,400),PILImage.NEAREST)
            image_io = StringIO.StringIO()
            image_data.save(image_io,'png')
            image_file = ContentFile(image_io.getvalue())

            qr_code.qr_image.save(qr_image_file_name,image_file)
                        
            qr_code.save()

            os.remove('/tmp/' + qr_image_file_name)

            return render(request,'sculptqr/homepage.html',{'form':image_form, 'qr_code':qr_code})
    
    else:
        image_form = ImageUploadForm()
        
    return render(request,'sculptqr/homepage.html',{'form':image_form,'qr_code':qr_code})


#def image_crop(request, qr_code_id):
#
#    #XXX Need error checking
#    crop_rect = eval(request.POST['crop_rect'])
#    target_size = eval(request.POST['target_size'])
#    qr_code = QRCode.objects.get(pk=qr_code_id)
#
#    (image_path,file_name) = os.path.split(qr_code.source_image.path)
#    (file_name_prefix,file_name_ext) = file_name.rsplit('.',1)
#
#    image_data = PILImage.open(qr_code.source_image)
#
#    # check if this is a qurious image as we need to adjust the names
#    #if image.role == Image.IMAGE_ROLE_QURIOUS_THUMBNAIL or image.role == Image.IMAGE_ROLE_QURIOUS_GALLERY:
#    #    is_lo_res = False
#    #    cropped_image_file_name = file_name_prefix + '_cropped-hd.jpg'
#    #    crop_and_save(image, image_data, cropped_image_file_name, crop_rect, target_size, is_lo_res)
#    #    # if this is a qurious image, also crop at half res
#    #    is_lo_res = True
#    #    target_size = (target_size[0]/2, target_size[1]/2)
#    #    cropped_image_file_name = file_name_prefix + '_cropped.jpg' # lo-res images just omit -hd suffix
#    #    crop_and_save(image, image_data, cropped_image_file_name, crop_rect, target_size, is_lo_res)
#    #else:
#    cropped_image_file_name = file_name_prefix + '_cropped.jpg'
#    is_lo_res = False
#    crop_and_save(qr_code, image_data, cropped_image_file_name, crop_rect, target_size, is_lo_res)
#
#    return redirect('sculptqr.views.stage2',qr_code_id=qr_code_id)
#    #return  HttpResponse('{"cropped_image":{"url":"%s","id":%d}}' % (qr_code.cropped_image.url, qr_code.id))
#
#
#def crop_and_save(qr_code, image_data, cropped_image_file_name, crop_rect, target_size, is_lo_res):
#    cropped_image_data = image_data.convert('L')
#    cropped_image_data = cropped_image_data.filter(ImageFilter.FIND_EDGES)
#    cropped_image_data = cropped_image_data.crop(crop_rect)
#    cropped_image_data.thumbnail(target_size,PILImage.ANTIALIAS)
#
#    cropped_image_io = StringIO.StringIO()
#    # Check if the image has an alpha layer and if so make it white.
#    if cropped_image_data.mode in ["RGBA","LA"] :
#        if cropped_image_data.mode == "RGBA" : index = 3
#        else : index = 1
#        cropped_image_data.load()
#        background = PILImage.new("RGB", cropped_image_data.size, (255, 255, 255))
#        background.paste(cropped_image_data, mask=cropped_image_data.split()[index]) # 3 is the alpha channel
#        background = background.convert('1')
#        background.save(cropped_image_io,'png')
#    else:
#        if cropped_image_data.mode != "RGB": cropped_image_data = cropped_image_data.convert("RGB")  # Convert GIF/PNG w/pallette to JPEG friendly RGB
#        #cropped_image_data = cropped_image_data.convert('1')
#        #cropped_image_data = cropped_image_data.filter(ImageFilter.FIND_EDGES)
#        cropped_image_data = cropped_image_data.point(lambda i: ((i > 10) and 1) or 255)
#        cropped_image_data = cropped_image_data.convert('1')
#        cropped_image_data.save(cropped_image_io,'png')
#    cropped_image_file = ContentFile(cropped_image_io.getvalue())
#    
#    qr_code.cropped_image.save(cropped_image_file_name,cropped_image_file)
#    qr_code.save()
