# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Adding model 'QRCode'
        db.create_table('sculptqr_qrcode', (
            ('id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('source_image', self.gf('django.db.models.fields.files.ImageField')(max_length=100)),
            ('source_image_width', self.gf('django.db.models.fields.IntegerField')()),
            ('source_image_height', self.gf('django.db.models.fields.IntegerField')()),
            ('cropped_image', self.gf('django.db.models.fields.files.ImageField')(max_length=100, null=True)),
            ('cropped_image_width', self.gf('django.db.models.fields.IntegerField')()),
            ('cropped_image_height', self.gf('django.db.models.fields.IntegerField')()),
            ('created_at', self.gf('django.db.models.fields.DateTimeField')(auto_now_add=True, blank=True)),
            ('updated_at', self.gf('django.db.models.fields.DateTimeField')(auto_now=True, blank=True)),
        ))
        db.send_create_signal('sculptqr', ['QRCode'])


    def backwards(self, orm):
        # Deleting model 'QRCode'
        db.delete_table('sculptqr_qrcode')


    models = {
        'sculptqr.qrcode': {
            'Meta': {'object_name': 'QRCode'},
            'created_at': ('django.db.models.fields.DateTimeField', [], {'auto_now_add': 'True', 'blank': 'True'}),
            'cropped_image': ('django.db.models.fields.files.ImageField', [], {'max_length': '100', 'null': 'True'}),
            'cropped_image_height': ('django.db.models.fields.IntegerField', [], {}),
            'cropped_image_width': ('django.db.models.fields.IntegerField', [], {}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'source_image': ('django.db.models.fields.files.ImageField', [], {'max_length': '100'}),
            'source_image_height': ('django.db.models.fields.IntegerField', [], {}),
            'source_image_width': ('django.db.models.fields.IntegerField', [], {}),
            'updated_at': ('django.db.models.fields.DateTimeField', [], {'auto_now': 'True', 'blank': 'True'})
        }
    }

    complete_apps = ['sculptqr']