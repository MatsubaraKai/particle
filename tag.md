# tag作成

# アノテーション付きでタグ作成
$ git tag -a v1.0.0 -m "CG4_02_"

# tagをpush

# 以下のどれでもタグをPUSHできる。

# 対象タグのみをPUSH
$ git push origin v1.0.0
# masterのPUSHと共に、このブランチに紐づく全タグをPUSH
$ git push origin master --tags
# masterのPUSHと共に、このブランチに紐づく全タグをPUSH
$ git push --tags

# tagのつけ直し
つけ直しは、tagを削除してから同じ名前で再作成することになる。