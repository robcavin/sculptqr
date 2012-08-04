from django.conf.urls import patterns, include, url
from django.conf import settings

urlpatterns = patterns('',
    # Examples:
    url(r'^$','sculptqr.views.homepage',{'qr_code_id':None}),
    url(r'^(?P<qr_code_id>\d+)/$','sculptqr.views.homepage'),
    #url(r'^qr_code/(?P<qr_code_id>\d+)/stage2','sculptqr.views.stage2'),
    #url(r'^qr_code/new/image_upload/$','sculptqr.views.image_upload'),
    #url(r'^qr_code/(?P<qr_code_id>\d+)/image_crop/','sculptqr.views.image_crop')
)

if settings.DEBUG:
    urlpatterns += patterns('',
        url(r'^media/(?P<path>.*)$', 'django.views.static.serve', {
            'document_root': settings.MEDIA_ROOT,
        }),
   )