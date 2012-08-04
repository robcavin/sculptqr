# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Adding field 'QRCode.url'
        db.add_column('sculptqr_qrcode', 'url',
                      self.gf('django.db.models.fields.URLField')(default='bumblebeejuice.com', max_length=200),
                      keep_default=False)

        # Adding field 'QRCode.code'
        db.add_column('sculptqr_qrcode', 'code',
                      self.gf('django.db.models.fields.CharField')(default=0, max_length=10),
                      keep_default=False)


    def backwards(self, orm):
        # Deleting field 'QRCode.url'
        db.delete_column('sculptqr_qrcode', 'url')

        # Deleting field 'QRCode.code'
        db.delete_column('sculptqr_qrcode', 'code')


    models = {
        'sculptqr.qrcode': {
            'Meta': {'object_name': 'QRCode'},
            'code': ('django.db.models.fields.CharField', [], {'max_length': '10'}),
            'created_at': ('django.db.models.fields.DateTimeField', [], {'auto_now_add': 'True', 'blank': 'True'}),
            'cropped_image': ('django.db.models.fields.files.ImageField', [], {'max_length': '100', 'null': 'True'}),
            'cropped_image_height': ('django.db.models.fields.IntegerField', [], {'null': 'True'}),
            'cropped_image_width': ('django.db.models.fields.IntegerField', [], {'null': 'True'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'qr_image': ('django.db.models.fields.files.ImageField', [], {'max_length': '100', 'null': 'True'}),
            'qr_image_height': ('django.db.models.fields.IntegerField', [], {'null': 'True'}),
            'qr_image_width': ('django.db.models.fields.IntegerField', [], {'null': 'True'}),
            'source_image': ('django.db.models.fields.files.ImageField', [], {'max_length': '100'}),
            'source_image_height': ('django.db.models.fields.IntegerField', [], {}),
            'source_image_width': ('django.db.models.fields.IntegerField', [], {}),
            'updated_at': ('django.db.models.fields.DateTimeField', [], {'auto_now': 'True', 'blank': 'True'}),
            'url': ('django.db.models.fields.URLField', [], {'max_length': '200'})
        }
    }

    complete_apps = ['sculptqr']