# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Adding field 'QRCode.qr_image'
        db.add_column('sculptqr_qrcode', 'qr_image',
                      self.gf('django.db.models.fields.files.ImageField')(max_length=100, null=True),
                      keep_default=False)

        # Adding field 'QRCode.qr_image_width'
        db.add_column('sculptqr_qrcode', 'qr_image_width',
                      self.gf('django.db.models.fields.IntegerField')(null=True),
                      keep_default=False)

        # Adding field 'QRCode.qr_image_height'
        db.add_column('sculptqr_qrcode', 'qr_image_height',
                      self.gf('django.db.models.fields.IntegerField')(null=True),
                      keep_default=False)


    def backwards(self, orm):
        # Deleting field 'QRCode.qr_image'
        db.delete_column('sculptqr_qrcode', 'qr_image')

        # Deleting field 'QRCode.qr_image_width'
        db.delete_column('sculptqr_qrcode', 'qr_image_width')

        # Deleting field 'QRCode.qr_image_height'
        db.delete_column('sculptqr_qrcode', 'qr_image_height')


    models = {
        'sculptqr.qrcode': {
            'Meta': {'object_name': 'QRCode'},
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
            'updated_at': ('django.db.models.fields.DateTimeField', [], {'auto_now': 'True', 'blank': 'True'})
        }
    }

    complete_apps = ['sculptqr']