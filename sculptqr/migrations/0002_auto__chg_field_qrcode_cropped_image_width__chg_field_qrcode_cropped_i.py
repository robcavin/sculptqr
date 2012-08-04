# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):

        # Changing field 'QRCode.cropped_image_width'
        db.alter_column('sculptqr_qrcode', 'cropped_image_width', self.gf('django.db.models.fields.IntegerField')(null=True))

        # Changing field 'QRCode.cropped_image_height'
        db.alter_column('sculptqr_qrcode', 'cropped_image_height', self.gf('django.db.models.fields.IntegerField')(null=True))

    def backwards(self, orm):

        # Changing field 'QRCode.cropped_image_width'
        db.alter_column('sculptqr_qrcode', 'cropped_image_width', self.gf('django.db.models.fields.IntegerField')(default=0))

        # Changing field 'QRCode.cropped_image_height'
        db.alter_column('sculptqr_qrcode', 'cropped_image_height', self.gf('django.db.models.fields.IntegerField')(default=0))

    models = {
        'sculptqr.qrcode': {
            'Meta': {'object_name': 'QRCode'},
            'created_at': ('django.db.models.fields.DateTimeField', [], {'auto_now_add': 'True', 'blank': 'True'}),
            'cropped_image': ('django.db.models.fields.files.ImageField', [], {'max_length': '100', 'null': 'True'}),
            'cropped_image_height': ('django.db.models.fields.IntegerField', [], {'null': 'True'}),
            'cropped_image_width': ('django.db.models.fields.IntegerField', [], {'null': 'True'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'source_image': ('django.db.models.fields.files.ImageField', [], {'max_length': '100'}),
            'source_image_height': ('django.db.models.fields.IntegerField', [], {}),
            'source_image_width': ('django.db.models.fields.IntegerField', [], {}),
            'updated_at': ('django.db.models.fields.DateTimeField', [], {'auto_now': 'True', 'blank': 'True'})
        }
    }

    complete_apps = ['sculptqr']