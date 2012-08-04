from django.conf.urls import patterns, include, url
from django.conf import settings

urlpatterns = patterns('',
    # Examples:
    url(r'^$','sculptqr.views.homepage',{'qr_code_id':None}),
    url(r'^(?P<qr_code_id>\d+)/$','sculptqr.views.homepage'),
    url(r'^c/(?P<code>\d+)/$','sculptqr.views.redirect_service')

)

if settings.DEBUG:
    urlpatterns += patterns('',
        url(r'^media/(?P<path>.*)$', 'django.views.static.serve', {
            'document_root': settings.MEDIA_ROOT,
        }),
   )