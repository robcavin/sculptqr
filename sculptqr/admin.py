from sculptqr import models
from django.contrib import admin

admin.site.register(models.QRCode,admin.ModelAdmin)
